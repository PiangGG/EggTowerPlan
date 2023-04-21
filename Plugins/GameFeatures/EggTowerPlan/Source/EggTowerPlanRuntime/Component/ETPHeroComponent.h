// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/LyraHeroComponent.h"
#include "ETPHeroComponent.generated.h"

/**
 * 
 */
UCLASS()
class EGGTOWERPLANRUNTIME_API UETPHeroComponent : public ULyraHeroComponent
{
	GENERATED_BODY()

public:
	UETPHeroComponent(const FObjectInitializer& ObjectInitializer);
	

	/** Overrides the camera from an active gameplay ability */
	void SetAbilityCameraMode(TSubclassOf<ULyraCameraMode> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle);

	/** Clears the camera override if it is set */
	void ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle);

	/** Adds mode-specific input config */
	void AddAdditionalInputConfig(const ULyraInputConfig* InputConfig);

	/** Removes a mode-specific input config if it has been added */
	void RemoveAdditionalInputConfig(const ULyraInputConfig* InputConfig);

	/** True if this is controlled by a real player and has progressed far enough in initialization where additional input bindings can be added */
	bool IsReadyToBindInputs() const;
	
	/** The name of the extension event sent via UGameFrameworkComponentManager when ability inputs are ready to bind */
	static const FName NAME_BindInputsNowETP;

	/** The name of this component-implemented feature */
	static const FName NAME_ActorFeatureNameETP;

	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;
	//~ End IGameFrameworkInitStateInterface interface

	
	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);
protected:

	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_LookStick(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);
	void Input_AutoRun(const FInputActionValue& InputActionValue);

	TSubclassOf<ULyraCameraMode> DetermineCameraMode() const;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Input")
	TObjectPtr<ULyraInputConfig> myInputConfig;
	
	static const FName NAME_BindInputs;
	
public:
	// Default camera mode used by player controlled pawns.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Camera")
	TSubclassOf<ULyraCameraMode> DefaultCameraMode;

	FVector LastFrameLocation = FVector(0, 0, 0);
	void SetLastFrameLocation(FVector Vector);
	FVector GetLastFrameLocation();
	FRotator LastFrameRotation = FRotator(0, 0, 0);
	void SetLastFrameRotation(FRotator Rotator);
	FRotator GetLastFrameRotation();

	//当前选择的建筑索引
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Build")
	int32 SelectedBuildIndex = -1;
	UFUNCTION(BlueprintCallable, Category = "Lyra|Build")
	int32 GetSelectedBuildIndex();
	UFUNCTION(BlueprintCallable, Category = "Lyra|Build")
	void SetSelectedBuildIndex(int32 Index);
};
