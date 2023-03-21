// Fill out your copyright notice in the Description page of Project Settings.


#include "TYCharacter.h"

#include "ControPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ATYCharacter::ATYCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 500.0f;
	SpringArmComponent->SetRelativeRotation(FRotator(-60.0f,-60.0f,0.0f));
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->bEnableCameraRotationLag = true;
	//SpringArmComponent->b

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
}

void ATYCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(APlayerController* PC = CastChecked<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 100);
		}
	}

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if(IA_MoveForward)
		{
			EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &ATYCharacter::MoveForward);
		}

		if(IA_MoveRight)
		{
			EnhancedInputComponent->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &ATYCharacter::MoveRight);
		}
		
	}
}
void ATYCharacter::MoveForward(const FInputActionValue& InputActionValue)
{
	/*if (GetController()&&InputActionValue.GetMagnitude()!=0.0f)
	{
		const FRotator Ratation = GetControlRotation();
		const FRotator YawRation = FRotator(0.0f,Ratation.Yaw,0.0f);
		const FVector Dirction = FRotationMatrix(YawRation).GetUnitAxis(EAxis::X);
		AddMovementInput(Dirction,InputActionValue.GetMagnitude());
	}*/
	if (CameraComponent&&InputActionValue.GetMagnitude()!=0.0f)
	{
		const FRotator Ratation = CameraComponent->GetComponentRotation();
		const FRotator YawRation = FRotator(0.0f,Ratation.Yaw,0.0f);
		const FVector Dirction = FRotationMatrix(YawRation).GetUnitAxis(EAxis::X);
		AddMovementInput(Dirction,InputActionValue.GetMagnitude());
	}
}

void ATYCharacter::MoveRight(const FInputActionValue& InputActionValue)
{
	/*if (GetController()&&InputActionValue.GetMagnitude()!=0.0f)
	{
		const FRotator Ratation = GetControlRotation();
		const FRotator YawRation = FRotator(0.0f,Ratation.Yaw,0.0f);
		const FVector Dirction = FRotationMatrix(YawRation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Dirction,InputActionValue.GetMagnitude());
	}*/
	if (CameraComponent&&InputActionValue.GetMagnitude()!=0.0f)
	{
		const FRotator Ratation =CameraComponent->GetComponentRotation();
		const FRotator YawRation = FRotator(0.0f,Ratation.Yaw,0.0f);
		const FVector Dirction = FRotationMatrix(YawRation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Dirction,InputActionValue.GetMagnitude());
	}
}