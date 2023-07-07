// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBuild.h"
#include "LyraGameplayTags.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraCombatSet.h"
#include "AbilitySystem/Attributes/LyraHealthSet.h"
#include "Character/LyraHealthComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "EggTowerPlanRuntime/Ability/Ability_ModeChange.h"
#include "EggTowerPlanRuntime/Component/AIManageComponent.h"
#include "EggTowerPlanRuntime/Inventory/Fragment/InventoryItemFragment_RTS.h"
#include "EggTowerPlanRuntime/World/Actor/BuildActor/BuildTipPlane.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameModes/LyraExperienceManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/LyraPlayerState.h"

ABaseBuild::ABaseBuild(const FObjectInitializer& ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<ULyraAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	HealthSet = CreateDefaultSubobject<ULyraHealthSet>(TEXT("HealthSet"));
	CombatSet = CreateDefaultSubobject<ULyraCombatSet>(TEXT("CombatSet"));
	
	HealthComponent = CreateDefaultSubobject<ULyraHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
	HealthComponent->OnDeathFinished.AddDynamic(this, &ThisClass::OnDeathFinished);
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootSceneComponent;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootSceneComponent);
	Mesh->bCastDynamicShadow = false;
	Mesh->bCastStaticShadow = false;
	
	CollsionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollsionComp"));
	CollsionComp->SetCollisionProfileName(FName("ETP_Damage_Build"));
	CollsionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollsionComp->bFillCollisionUnderneathForNavmesh = false;
	CollsionComp->SetSphereRadius(DamageSphereRadius);
	CollsionComp->SetupAttachment(RootSceneComponent);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionProfileName(FName("ETP_BuildProfile"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->bFillCollisionUnderneathForNavmesh = true;
	BoxComponent->SetBoxExtent(BoxSizeVector);
	BoxComponent->SetupAttachment(CollsionComp);
	BoxComponent->SetRelativeLocation(FVector(CollsionComp->GetRelativeLocation().X,CollsionComp->GetRelativeLocation().Y,CollsionComp->GetRelativeLocation().Z+BoxSizeVector.Z-1.0f));
	BoxComponent->SetupAttachment(RootSceneComponent);
	
	
	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	HPBar->SetupAttachment(RootSceneComponent);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(Mesh,"AttackSocket");
	NiagaraComponent->bAutoActivate = false;

	CurrentAIState = EDefenseState::Building;
}

void ABaseBuild::BeginPlay()
{
	Super::BeginPlay();
	if (GetNetMode() != NM_Client)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);
		ULyraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}
	if (BoxComponent)
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::UnitBeginOverlap);
		BoxComponent->OnComponentEndOverlap.AddDynamic(this,&ThisClass::UnitEndOverlap);
	}
	if (HealthComponent&&AbilitySystemComponent)
	{
		HealthComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
	}
}

void ABaseBuild::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if(GameMode)
	{
		UAIManageComponent* AIManageComponent = GameMode->FindComponentByClass<UAIManageComponent>();
		if(AIManageComponent)
		{
			AIManageComponent->UnRegisterBuild(this);
		}
	}
	Super::EndPlay(EndPlayReason);
}

void ABaseBuild::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

UAbilitySystemComponent* ABaseBuild::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseBuild::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseBuild::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void ABaseBuild::Destroyed()
{
	Super::Destroyed();
}

void ABaseBuild::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& InteractionBuilder)
{
	InteractionBuilder.AddInteractionOption(Option);
}

void ABaseBuild::OnExperienceLoaded(const ULyraExperienceDefinition* Definition)
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, ALyraPlayerState::NAME_LyraAbilityReady);
	
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if(GameMode)
	{
		UAIManageComponent* AIManageComponent = GameMode->FindComponentByClass<UAIManageComponent>();
		if(AIManageComponent)
		{
			AIManageComponent->RegisterBuild(this);
		}
	}
}

void ABaseBuild::OnDeathFinished(AActor* OwningActor)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}

void ABaseBuild::DisableMovementAndCollision()
{
}

void ABaseBuild::DestroyDueToDeath()
{
	K2_OnDeathFinished();

	UninitAndDestroy();
}

void ABaseBuild::UninitAndDestroy()
{
}

void ABaseBuild::OnDeathStarted(AActor* OwningActor)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		//DetachFromControllerPendingDestroy();
		SetLifeSpan(0.1f);
	}
	if (AbilitySystemComponent)
	{
		if (AbilitySystemComponent->GetAvatarActor() == this)
		{
			if (!AbilitySystemComponent)
			{
				return;
			}
			if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
			{
				FGameplayTagContainer AbilityTypesToIgnore;
				AbilityTypesToIgnore.AddTag(LyraGameplayTags::Ability_Behavior_SurvivesDeath);

				AbilitySystemComponent->CancelAbilities(nullptr, &AbilityTypesToIgnore);
				AbilitySystemComponent->ClearAbilityInput();
				AbilitySystemComponent->RemoveAllGameplayCues();

				if (AbilitySystemComponent->GetOwnerActor() != nullptr)
				{
					AbilitySystemComponent->SetAvatarActor(nullptr);
				}
				else
				{
					AbilitySystemComponent->ClearActorInfo();
				}
			}

			AbilitySystemComponent = nullptr;
		}
	}

	SetActorHiddenInGame(true);
}

void ABaseBuild::UnitBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ABuildTipPlane>(OtherActor))
	{
		Cast<ABuildTipPlane>(OtherActor)->UnitBeginOverlap(OtherComp);
	}
}

void ABaseBuild::UnitEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ABuildTipPlane>(OtherActor))
	{
		Cast<ABuildTipPlane>(OtherActor)->UnitEndOverlap(OtherComp);
	}
}

UMaterialInterface* ABaseBuild::GetInteractioningMaterial_Implementation()
{
	return Overlaymaterial;
}

void ABaseBuild::SetSelfInteractioning_Implementation(bool bInteractioning)
{
	if (bInteractioning)
	{
		TArray<UActorComponent*> Meshs;
		GetComponents(USkeletalMeshComponent::StaticClass(),Meshs);
		for (auto ActorComponent : Meshs)
		{
			if (USkeletalMeshComponent * MeshItem = Cast<USkeletalMeshComponent>(ActorComponent))
			{
				MeshItem->SetOverlayMaterial(GetInteractioningMaterial_Implementation());
			}
		}

		TArray<UActorComponent*> Meshs2;
		GetComponents(UStaticMeshComponent::StaticClass(),Meshs2);
		for (auto ActorComponent : Meshs2)
		{
			if (UStaticMeshComponent * MeshItem = Cast<UStaticMeshComponent>(ActorComponent))
			{
				MeshItem->SetOverlayMaterial(GetInteractioningMaterial_Implementation());
			}
		}
	}
	else
	{
		TArray<UActorComponent*> Meshs;
		GetComponents(USkeletalMeshComponent::StaticClass(),Meshs);
		for (auto ActorComponent : Meshs)
		{
			if (USkeletalMeshComponent * MeshItem = Cast<USkeletalMeshComponent>(ActorComponent))
			{
				MeshItem->SetOverlayMaterial(nullptr);
			}
		}

		TArray<UActorComponent*> Meshs2;
		GetComponents(UStaticMeshComponent::StaticClass(),Meshs2);
		for (auto ActorComponent : Meshs2)
		{
			if (UStaticMeshComponent * MeshItem = Cast<UStaticMeshComponent>(ActorComponent))
			{
				MeshItem->SetOverlayMaterial(nullptr);
			}
		}
	}
}

void ABaseBuild::SetCurrentAiState(EDefenseState AIState)
{
	if(CurrentAIState == AIState)
	{
		return;
	}
	
	CurrentAIState = AIState;
		
	switch (CurrentAIState)
	{
		case EDefenseState::Building: break;
		case EDefenseState::EIdle:
			if (BoxComponent)
			{
				BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			}
			if (CollsionComp)
			{
				CollsionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
			break;
		
		case EDefenseState::EBeHit: break;
		case EDefenseState::EUsingAbility: break;
		case EDefenseState::EAttack: break;
		case EDefenseState::ENULL: break;
		default: ;
	}
	K2_OnSetCurrentAiState(AIState);
}

void ABaseBuild::SetGenericTeamId(const FGenericTeamId& NewTeamID)
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

FGenericTeamId ABaseBuild::GetGenericTeamId() const
{
	return TeamId;
}

FOnLyraTeamIndexChangedDelegate* ABaseBuild::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

void ABaseBuild::AttackStart_Implementation(FName Selection, FName AttackSocket)
{
	FTimerDelegate UpdateAttackDelegate = FTimerDelegate::CreateUObject(this, &ABaseBuild::UpdateAttack);

	if (IsValid(ItemDefinition))
	{
		const ULyraInventoryItemFragment* Fragment = ULyraInventoryFunctionLibrary::FindItemDefinitionFragment(ItemDefinition,UInventoryItemFragment_RTS::StaticClass());
		if (Fragment&&Cast<UInventoryItemFragment_RTS>(Fragment))
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_Attacking,UpdateAttackDelegate,Cast<UInventoryItemFragment_RTS>(Fragment)->AttackRate,true);

			if (GetTargetActor())
			{
				GetTargetActor()->OnDestroyed.AddDynamic(this,&ThisClass::ABaseBuild::OnTargetOnDestroyed);
			}
			return;
		}
	}
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Attacking,UpdateAttackDelegate,1.0f,true);

	if (GetTargetActor())
	{
		GetTargetActor()->OnDestroyed.AddDynamic(this,&ThisClass::ABaseBuild::OnTargetOnDestroyed);
	}
}

void ABaseBuild::Attack_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
}

void ABaseBuild::AttackEnd_Implementation(FName Selection)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Attacking);
	if(NiagaraComponent)
	{
		NiagaraComponent->Deactivate();
	}
	SetCurrentAiState(EDefenseState::EIdle);
}

void ABaseBuild::UpdateAttack_Implementation()
{
	if (GetTargetActor())
	{
		if (NiagaraComponent)
		{
			NiagaraComponent->Activate();
		}
		ULyraAbilitySystemComponent* ASC = Cast<ULyraAbilitySystemComponent>(GetTargetActor()->GetComponentByClass(ULyraAbilitySystemComponent::StaticClass()));
		if(ASC)
		{
			ASC->BP_ApplyGameplayEffectToTarget(GameplayEffect,ASC,1,FGameplayEffectContextHandle());
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Attacking);
		if(NiagaraComponent)
		{
			NiagaraComponent->Deactivate();
		}
	}
}

void ABaseBuild::OnRep_CurrentAIState(EDefenseState AIState)
{
	
}

void ABaseBuild::OnTargetOnDestroyed(AActor* DestroyedActor)
{
	AttackEnd_Implementation("");
}

