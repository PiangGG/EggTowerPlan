// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/Abilities/LyraGameplayAbility_Interact.h"
#include "ETPGameplayAbility_Interact.generated.h"

/**
 * 
 */
UCLASS()
class EGGTOWERPLANRUNTIME_API UETPGameplayAbility_Interact : public ULyraGameplayAbility_Interact
{
	GENERATED_BODY()
public:

	UETPGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void UpdateInteractions(const TArray<FInteractionOption>& InteractiveOptions)override;

	virtual void TriggerInteraction()override;
};
