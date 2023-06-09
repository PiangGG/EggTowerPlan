// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EGGTOWERPLANRUNTIME_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=CombatInterface)
	UAnimMontage* GetAnimMontage();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=CombatInterface)
	float GetMaxAttackLength();
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=CombatInterface)
	void AttackStart(FName Selection,FName AttackSocket = "");

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=CombatInterface)
	void AttackEnd(FName Selection);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=CombatInterface)
	void BeHit(FName Selection);
};
