// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "StateComponent.generated.h"

/**
 * 
 */
class AETPCharacter;
UCLASS()
class EGGTOWERPLANRUNTIME_API UStateComponent : public UControllerComponent
{
	GENERATED_BODY()
public:
	UStateComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PCState")
	EControllerState State;

	UFUNCTION(BlueprintCallable,Category="PCState")
	EControllerState GetControllerState();

	UFUNCTION(BlueprintCallable,Category="PCState")
	void SetControllerState(EControllerState ControllerState);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PCState")
	TSubclassOf<ABaseCharacter> Character_RTS_Class;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PCState")
	ABaseCharacter* Character_RTS;
	void ChangeStateRTS();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PCState")
	TSubclassOf<ABaseCharacter> Character_Moba_Class;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PCState")
	ABaseCharacter* Character_Moba;
	void ChangeStateMoba();

};
