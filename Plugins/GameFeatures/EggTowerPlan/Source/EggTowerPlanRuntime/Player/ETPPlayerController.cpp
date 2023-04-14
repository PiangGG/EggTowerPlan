// Fill out your copyright notice in the Description page of Project Settings.


#include "ETPPlayerController.h"

#include "EggTowerPlanRuntime/Character/BaseCharacter.h"

AETPPlayerController::AETPPlayerController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

ULyraAbilitySystemComponent* AETPPlayerController::GetLyraAbilitySystemComponent() const
{
	if (GetCharacter())
	{
		ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetCharacter());
		if (BaseCharacter)
		{
			return BaseCharacter->GetLyraAbilitySystemComponent();
		}
	}
	return Super::GetLyraAbilitySystemComponent();
}
