// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraCombatSet.h"
#include "AbilitySystem/Attributes/LyraHealthSet.h"
#include "Character/LyraCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "EggTowerPlanRuntime/Tool/EnumLib.h"
#include "GameModes/LyraExperienceManagerComponent.h"
#include "Player/LyraPlayerState.h"

ABaseEnemy::ABaseEnemy(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<ULyraCharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<ULyraAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttackBoxCollsion = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("UBoxComponent"));
	AttackBoxCollsion->SetIsReplicated(true);
	AttackBoxCollsion->SetupAttachment(GetMesh(),DefaultAttackSocket);
	AttackBoxCollsion->SetCollisionProfileName(FName("ETP_BuildProfile"));
	AttackBoxCollsion->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	HealthSet = CreateDefaultSubobject<ULyraHealthSet>(TEXT("HealthSet"));
	CombatSet = CreateDefaultSubobject<ULyraCombatSet>(TEXT("CombatSet"));

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	check(AttackBoxCollsion);
	AttackBoxCollsion->OnComponentHit.AddDynamic(this,&ThisClass::Attack);
}

void ABaseEnemy::OnExperienceLoaded(const ULyraExperienceDefinition*)
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, ALyraPlayerState::NAME_LyraAbilityReady);
}

UAbilitySystemComponent* ABaseEnemy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ABaseEnemy::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	Super::GetActorEyesViewPoint(OutLocation, OutRotation);
}

void ABaseEnemy::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (GetController() == nullptr)
	{
		if (HasAuthority())
		{
			const FGenericTeamId OldTeamID = TeamId;
			TeamId = NewTeamID;

			//改变队伍的 代理调用 
			const int32 OldTeamIndex = GenericTeamIdToInteger(OldTeamID); 
			const int32 NewTeamIndex = GenericTeamIdToInteger(NewTeamID);
			GetOnTeamIndexChangedDelegate()->Broadcast(this,OldTeamIndex,NewTeamIndex);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("You can't set the team ID on a zombie (%s) except on the authority"), *GetPathNameSafe(this));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("You can't set the team ID on a possessed zombie (%s); it's driven by the associated controller"), *GetPathNameSafe(this));
	}
}

FGenericTeamId ABaseEnemy::GetGenericTeamId() const
{
	return TeamId;
}

FOnLyraTeamIndexChangedDelegate* ABaseEnemy::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	//注册消息，加载能力等
	if (GetNetMode() != NM_Client)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);
		ULyraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}
	
	//僵尸不会触发任何 重叠事件， 移动组件禁用物理量更新
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetCapsuleComponent()->SetShouldUpdatePhysicsVolume(false);
	GetCharacterMovement()->bComponentShouldUpdatePhysicsVolume = false;
	//GetCharacterMovement()->bAlwaysCheckFloor = false;
}

void ABaseEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ABaseEnemy::SetCurrentAiState(EEnemyState AIState)
{
	if(CurrentAIState == AIState)
	{
		return;
	}
	
	CurrentAIState = AIState;
		
	switch (CurrentAIState)
	{
		case EEnemyState::EIdle: break;
		case EEnemyState::EMoveToTarget: break;
		case EEnemyState::EBeHit: break;
		case EEnemyState::EUsingAbility: break;
		case EEnemyState::EAttack: break;
		case EEnemyState::ENULL: break;
		default: ;
	}
	
	K2_OnSetCurrentAiState(AIState);
}

void ABaseEnemy::OnRep_CurrentAIState(EEnemyState AIState)
{
	
}

UAnimMontage* ABaseEnemy::GetAnimMontage_Implementation()
{
	return AnimMontage;
}

float ABaseEnemy::GetMaxAttackLength_Implementation()
{
	return MaxAttackLength;
}

void ABaseEnemy::AttackStart_Implementation(FName Selection,FName AttackSocket)
{
	/*if (AttackBoxCollsion)
	{
		if (AttackSocket.IsValid())
		{
			AttackBoxCollsion->SetupAttachment(GetMesh(),AttackSocket);
		}
		AttackBoxCollsion->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}*/
	FTimerDelegate UpdateAttackDelegate = FTimerDelegate::CreateUObject(this, &ABaseEnemy::UpdateAttack,AttackSocket);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Attacking,UpdateAttackDelegate,0.1f,true);
}

void ABaseEnemy::Attack_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//if (Hit.bBlockingHit&&OtherActor)
	//{
	//	ULyraAbilitySystemComponent* ASC = Cast<ULyraAbilitySystemComponent>(OtherActor->GetComponentByClass(ULyraAbilitySystemComponent::StaticClass()));
	//	{
	//		ASC->BP_ApplyGameplayEffectToTarget(GameplayEffect,ASC,1,FGameplayEffectContextHandle());
	//		UE_LOG(LogTemp,Warning,TEXT("%s"),*OtherActor->GetName());
	//		AttackEnd_Implementation("");
	//	}
	//}
}

void ABaseEnemy::AttackEnd_Implementation(FName Selection)
{
	//AttackBoxCollsion->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Attacking);
}

void ABaseEnemy::UpdateAttack(FName AttackSocket)
{
	TArray<FHitResult>HitResults;
	FVector StartLocation = GetMesh()->GetSocketLocation(AttackSocket);
	FVector EndLocation = StartLocation+GetActorForwardVector()*100.0f;

	FCollisionShape ColSphere = FCollisionShape::MakeSphere(50.0);
	DrawDebugCylinder(GetWorld(),StartLocation,EndLocation,50.0,8,FColor::Green,false,3.0f);
	GetWorld()->SweepMultiByChannel(HitResults,StartLocation,EndLocation,FQuat::Identity,ECollisionChannel::ECC_GameTraceChannel6,ColSphere);
	if (!HitResults.IsEmpty())
	{
		for (auto HitResult : HitResults)
		{
			ULyraAbilitySystemComponent* ASC = Cast<ULyraAbilitySystemComponent>(HitResult.GetActor()->GetComponentByClass(ULyraAbilitySystemComponent::StaticClass()));
			{
				ASC->BP_ApplyGameplayEffectToTarget(GameplayEffect,ASC,1,FGameplayEffectContextHandle());
				UE_LOG(LogTemp,Warning,TEXT("%s"),*HitResult.GetActor()->GetName());
			}
		}
		
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Attacking);
	}
}

void ABaseEnemy::BeHit_Implementation(FName Selection)
{
	SetCurrentAiState(EEnemyState::EBeHit);
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
