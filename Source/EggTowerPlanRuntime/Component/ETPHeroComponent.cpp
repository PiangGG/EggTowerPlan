// Fill out your copyright notice in the Description page of Project Settings.


#include "ETPHeroComponent.h"

#include "EnhancedInputSubsystems.h"
#include "LyraGameplayTags.h"
#include "Camera/LyraCameraComponent.h"
#include "Camera/LyraCameraMode.h"
#include "Character/LyraCharacter.h"
#include "Components/GameFrameworkComponentManager.h"
#include "EggTowerPlanRuntime/Character/BaseCharacter.h"
#include "Input/LyraInputComponent.h"
#include "Player/LyraLocalPlayer.h"
#include "Player/LyraPlayerController.h"
#include "Player/LyraPlayerState.h"
#include "GameFramework/PlayerInput.h"
#include "EnhancedPlayerInput.h"
#include "PlayerMappableInputConfig.h"
namespace ETPRTS
{
	static const float LookYawRate = 300.0f;
	static const float LookPitchRate = 165.0f;
}

UETPHeroComponent::UETPHeroComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

void UETPHeroComponent::SetAbilityCameraMode(TSubclassOf<ULyraCameraMode> CameraMode,
	const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (CameraMode)
	{
		AbilityCameraMode = CameraMode;
		AbilityCameraModeOwningSpecHandle = OwningSpecHandle;
	}
}

void UETPHeroComponent::ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (AbilityCameraModeOwningSpecHandle == OwningSpecHandle)
	{
		AbilityCameraMode = nullptr;
		AbilityCameraModeOwningSpecHandle = FGameplayAbilitySpecHandle();
	}
}

void UETPHeroComponent::AddAdditionalInputConfig(const ULyraInputConfig* InputConfig)
{
	TArray<uint32> BindHandles;

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	ULyraInputComponent* LyraIC = Pawn->FindComponentByClass<ULyraInputComponent>();
	check(LyraIC);

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	LyraIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
}

void UETPHeroComponent::RemoveAdditionalInputConfig(const ULyraInputConfig* InputConfig)
{
	
}

bool UETPHeroComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}

bool UETPHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	return true;
	//return Super::CanChangeInitState(Manager, CurrentState, DesiredState);
}

void UETPHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	const FLyraGameplayTags& InitTags = FLyraGameplayTags::Get();
	if (CurrentState == FLyraGameplayTags::Get().InitState_DataAvailable && DesiredState == FLyraGameplayTags::Get().InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ALyraPlayerState* LyraPS = GetPlayerState<ALyraPlayerState>();
		if (!ensure(Pawn && LyraPS))
		{
			return;
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();

		
		if (ALyraPlayerController* LyraPC = GetController<ALyraPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}

		if (bIsLocallyControlled)
		{
			if (ULyraCameraComponent* CameraComponent = ULyraCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}
}

void UETPHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	//Super::OnActorInitStateChanged(Params);
}

void UETPHeroComponent::CheckDefaultInitialization()
{
	//Super::CheckDefaultInitialization();
}

void UETPHeroComponent::OnRegister()
{
	Super::OnRegister();
}

void UETPHeroComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UETPHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UETPHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULyraLocalPlayer* LP = Cast<ULyraLocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();
	
	if (const ULyraInputConfig* InputConfig = myInputConfig)
	{
		const FLyraGameplayTags& GameplayTags = FLyraGameplayTags::Get();
	
		// Register any default input configs with the settings so that they will be applied to the player during AddInputMappings
		for (const FMappableConfigPair& Pair : DefaultInputConfigs)
		{
			if (Pair.bShouldActivateAutomatically && Pair.CanBeActivated())
			{
				FModifyContextOptions Options = {};
				Options.bIgnoreAllPressedKeysUntilRelease = false;
				// Actually add the config to the local player							
				Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);	
			}
		}
		
		ULyraInputComponent* LyraIC = CastChecked<ULyraInputComponent>(PlayerInputComponent);
		LyraIC->AddInputMappings(InputConfig, Subsystem);

		TArray<uint32> BindHandles;
		LyraIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

		LyraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
		LyraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
		LyraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
		LyraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ false);
		LyraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_AutoRun, ETriggerEvent::Triggered, this, &ThisClass::Input_AutoRun, /*bLogIfNotFound=*/ false);
	}
	

	/*if (ensure(!bReadyToBindInputs))
	{
		bReadyToBindInputs = true;
	}*/
 
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);

}

void UETPHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (ABaseCharacter* Character = GetPawn<ABaseCharacter>())
	{
		if (ULyraAbilitySystemComponent* LyraASC = Character->GetLyraAbilitySystemComponent())
		{
			LyraASC->AbilityInputTagPressed(InputTag);
		}	
	}
}

void UETPHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	if (ABaseCharacter* Character = GetPawn<ABaseCharacter>())
	{
		if (ULyraAbilitySystemComponent* LyraASC = Character->GetLyraAbilitySystemComponent())
		{
			LyraASC->AbilityInputTagReleased(InputTag);
		}	
	}
}

void UETPHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	// If the player has attempted to move again then cancel auto running
	if (ALyraPlayerController* LyraController = Cast<ALyraPlayerController>(Controller))
	{
		LyraController->SetIsAutoRunning(false);
	}
	
	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UETPHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

void UETPHeroComponent::Input_LookStick(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	const UWorld* World = GetWorld();
	check(World);

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X * ETPRTS::LookYawRate * World->GetDeltaSeconds());
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y * ETPRTS::LookPitchRate * World->GetDeltaSeconds());
	}
}

void UETPHeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (ALyraCharacter* Character = GetPawn<ALyraCharacter>())
	{
		Character->ToggleCrouch();
	}
}

void UETPHeroComponent::Input_AutoRun(const FInputActionValue& InputActionValue)
{
	if (APawn* Pawn = GetPawn<APawn>())
	{
		if (ALyraPlayerController* Controller = Cast<ALyraPlayerController>(Pawn->GetController()))
		{
			// Toggle auto running
			Controller->SetIsAutoRunning(!Controller->GetIsAutoRunning());
		}	
	}
}

TSubclassOf<ULyraCameraMode> UETPHeroComponent::DetermineCameraMode() const
{
	if (DefaultCameraMode)
	{
		return DefaultCameraMode;
	}

	return nullptr;
}

void UETPHeroComponent::SetLastFrameLocation(FVector Vector)
{
	LastFrameLocation = Vector;
}

FVector UETPHeroComponent::GetLastFrameLocation()
{
	return LastFrameLocation;
}

FRotator UETPHeroComponent::GetLastFrameRotation()
{
	return LastFrameRotation;
}

void UETPHeroComponent::SetLastFrameRotation(FRotator Rotator)
{
	LastFrameRotation = Rotator;
}
