// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ControPawn.generated.h"

UCLASS()
class EGGTOWERPLANRUNTIME_API AControPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AControPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Component
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Comonent|Root")
	USceneComponent * RootCompment;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Comonent|Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Comonent|Camera")
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Comonent|Movement")
	UFloatingPawnMovement* Movement;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Component|Arrow")
	UArrowComponent* ArrowComponent;
	//移动
	void MoveForward(const FInputActionValue& InputActionValue);
	void MoveRight(const FInputActionValue& InputActionValue);

	void Move(const FInputActionValue& InputActionValue);
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Base|MoveSize")
	float BaseMoveSize = 3.0f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Base|MoveSize")
	float CurrentMoveSize = 3.0f;
	//推拉摄像机
	void PushCamera(const FInputActionValue& InputActionValue);

	void OnClickStarted(const FInputActionValue& InputActionValue);
	void OnClickCompleted(const FInputActionValue& InputActionValue);
	
	//相机偏移
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Camera|Offsize")
	float CameraOffsize_Max = 500.0f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Camera|Offsize")
	float CameraOffsize_Current = 0.0f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Camera|Offsize")
	float CameraOffsize_Rate = 50.0f;

	//Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="EnhancedInput", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="EnhancedInput|Action", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> IA_MoveForward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="EnhancedInput|Action", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> IA_MoveRight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="EnhancedInput|Action", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> IA_PushCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="EnhancedInput|Action", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> IA_OnClick;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="EnhancedInput|Action", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> IA_Move;
};



