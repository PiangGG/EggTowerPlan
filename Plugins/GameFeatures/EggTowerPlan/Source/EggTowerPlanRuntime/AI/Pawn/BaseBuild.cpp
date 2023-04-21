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
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameModes/LyraExperienceManagerComponent.h"
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
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	HealthComponent = CreateDefaultSubobject<ULyraHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
	HealthComponent->OnDeathFinished.AddDynamic(this, &ThisClass::OnDeathFinished);

	CollsionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollsionComp"));
	CollsionComp->SetCollisionProfileName(FName("Interactable_BlockDynamic"));
	CollsionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollsionComp->bFillCollisionUnderneathForNavmesh = false;
	RootComponent = CollsionComp;
	Mesh->SetupAttachment(CollsionComp);
	Mesh->bCastDynamicShadow = false;
	Mesh->bCastStaticShadow = false;
	//CollsionComp->SetSphereRadius(128.0);

	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	HPBar->SetupAttachment(CollsionComp);
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

	if (HealthComponent&&AbilitySystemComponent)
	{
		HealthComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
	}
}

void ABaseBuild::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	
}

void ABaseBuild::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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
				AbilityTypesToIgnore.AddTag(FLyraGameplayTags::Get().Ability_Behavior_SurvivesDeath);

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

void ABaseBuild::OnRep_CurrentAIState(EDefenseState AIState)
{
	
}

