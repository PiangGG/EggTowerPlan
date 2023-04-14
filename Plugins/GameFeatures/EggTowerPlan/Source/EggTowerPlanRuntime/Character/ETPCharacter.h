// Fill out your copyright notice in the Description page of Project Settings.
/*
 * 继承自LyraCharactor
 */
#pragma once

#include "CoreMinimal.h"
#include "Character/LyraCharacter.h"
#include "ETPCharacter.generated.h"

class ULyraAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class EGGTOWERPLANRUNTIME_API AETPCharacter : public ALyraCharacter
{
	GENERATED_BODY()
	
public:
	AETPCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnAbilitySystemInitialized() override;
	
	virtual void OnAbilitySystemUninitialized() override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

};
