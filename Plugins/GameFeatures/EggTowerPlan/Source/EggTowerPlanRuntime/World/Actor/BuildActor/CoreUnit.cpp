// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreUnit.h"

#include "LyraGameplayTags.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraHealthSet.h"
#include "Character/LyraHealthComponent.h"
#include "EggTowerPlanRuntime/Ability/Ability_ModeChange.h"
#include "EggTowerPlanRuntime/Tool/StructLib.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Player/LyraPlayerState.h"

//UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_CoreUnit_Change);
UE_DEFINE_GAMEPLAY_TAG(TAG_CoreUnit_Change,"ETP.CoreUnit.ChangeCoreUnitNumber");

// Sets default values
ACoreUnit::ACoreUnit(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	
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

void ACoreUnit::OnExperienceLoaded(const ULyraExperienceDefinition* Definition)
{
	Super::OnExperienceLoaded(Definition);

	if (GetWorld())
	{
		FChangeCoreUnitMessage Message;
		Message.CoreUnit = this;
		Message.CoreUnitChange = ECoreUnitChange::ECoreUnitAdd;
		UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
		MessageSystem.BroadcastMessage(TAG_CoreUnit_Change, Message);
	}
}

