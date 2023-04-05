// Fill out your copyright notice in the Description page of Project Settings.


#include "ETPCharacter.h"

#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "Character/LyraHeroComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "EggTowerPlanRuntime/Ability/Ability_ModeChange.h"
#include "Player/LyraPlayerState.h"

AETPCharacter::AETPCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

void AETPCharacter::OnAbilitySystemInitialized()
{
	Super::OnAbilitySystemInitialized();
}

void AETPCharacter::OnAbilitySystemUninitialized()
{
	Super::OnAbilitySystemUninitialized();
}

void AETPCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (ULyraHeroComponent* LyraHeroComp= Cast<ULyraHeroComponent>(GetComponentByClass(ULyraHeroComponent::StaticClass())))
	{
		LyraHeroComp->HandleControllerChanged();
	}
	
}

void AETPCharacter::UnPossessed()
{
	Super::UnPossessed();
	
	if (ULyraHeroComponent* LyraHeroComp= Cast<ULyraHeroComponent>(GetComponentByClass(ULyraHeroComponent::StaticClass())))
	{
		LyraHeroComp->HandleControllerChanged();
	}
}
