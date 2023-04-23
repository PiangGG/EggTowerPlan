// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TYCharacter.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
UCLASS()
class EGGTOWERPLANRUNTIME_API ATYCharacter : public AModularCharacter
{
	GENERATED_BODY()

	ATYCharacter();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Comonent|Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Comonent|Camera")
	USpringArmComponent* SpringArmComponent;

	
	//Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="EnhancedInput", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="EnhancedInput|Action", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> IA_MoveForward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="EnhancedInput|Action", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> IA_MoveRight;

	//
	void MoveForward(const FInputActionValue& InputActionValue);

	
	void MoveRight(const FInputActionValue& InputActionValue);
private:
};
