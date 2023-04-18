// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreUnit.h"

#include "LyraGameplayTags.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraCombatSet.h"
#include "AbilitySystem/Attributes/LyraHealthSet.h"
#include "Character/LyraHealthComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "EggTowerPlanRuntime/Ability/Ability_ModeChange.h"
#include "EggTowerPlanRuntime/Tool/StructLib.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameModes/LyraExperienceManagerComponent.h"
#include "Player/LyraPlayerState.h"

//UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_CoreUnit_Change);
UE_DEFINE_GAMEPLAY_TAG(TAG_CoreUnit_Change,"ETP.CoreUnit.ChangeCoreUnitNumber");
// Sets default values
ACoreUnit::ACoreUnit(const FObjectInitializer& ObjectInitializer)
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

// Called when the game starts or when spawned
void ACoreUnit::BeginPlay()
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

void ACoreUnit::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetWorld())
	{
		FChangeCoreUnitMessage Message;
		Message.CoreUnit = this;
		Message.CoreUnitChange = ECoreUnitChange::ECoreUnitRemove;
		UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
		MessageSystem.BroadcastMessage(TAG_CoreUnit_Change, Message);
	}
}

// Called every frame
void ACoreUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACoreUnit::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void ACoreUnit::Destroyed()
{
	Super::Destroyed();

	
}

void ACoreUnit::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
                                         FInteractionOptionBuilder& InteractionBuilder)
{
	InteractionBuilder.AddInteractionOption(Option);
}

void ACoreUnit::OnExperienceLoaded(const ULyraExperienceDefinition*)
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, ALyraPlayerState::NAME_LyraAbilityReady);

	if (GetWorld())
	{
		FChangeCoreUnitMessage Message;
		Message.CoreUnit = this;
		Message.CoreUnitChange = ECoreUnitChange::ECoreUnitAdd;
		UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
		MessageSystem.BroadcastMessage(TAG_CoreUnit_Change, Message);
	}
}

void ACoreUnit::OnDeathFinished(AActor* OwningActor)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}

void ACoreUnit::OnDeathStarted(AActor* OwningActor)
{
	
}

UMaterialInterface* ACoreUnit::GetInteractioningMaterial_Implementation()
{
	return Overlaymaterial;
}

void ACoreUnit::SetSelfInteractioning_Implementation(bool bInteractioning)
{
	if (bInteractioning)
	{
		TArray<UActorComponent*> Meshs = GetComponentsByClass(USkeletalMeshComponent::StaticClass());
		for (auto ActorComponent : Meshs)
		{
			if (USkeletalMeshComponent * MeshItem = Cast<USkeletalMeshComponent>(ActorComponent))
			{
				MeshItem->SetOverlayMaterial(GetInteractioningMaterial_Implementation());
			}
		}

		TArray<UActorComponent*> Meshs2 = GetComponentsByClass(UStaticMeshComponent::StaticClass());
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
		TArray<UActorComponent*> Meshs = GetComponentsByClass(USkeletalMeshComponent::StaticClass());
		for (auto ActorComponent : Meshs)
		{
			if (USkeletalMeshComponent * MeshItem = Cast<USkeletalMeshComponent>(ActorComponent))
			{
				MeshItem->SetOverlayMaterial(nullptr);
			}
		}

		TArray<UActorComponent*> Meshs2 = GetComponentsByClass(UStaticMeshComponent::StaticClass());
		for (auto ActorComponent : Meshs2)
		{
			if (UStaticMeshComponent * MeshItem = Cast<UStaticMeshComponent>(ActorComponent))
			{
				MeshItem->SetOverlayMaterial(nullptr);
			}
		}
	}
}

void ACoreUnit::DisableMovementAndCollision()
{
}

void ACoreUnit::DestroyDueToDeath()
{
	K2_OnDeathFinished();

	UninitAndDestroy();
}

void ACoreUnit::UninitAndDestroy()
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

