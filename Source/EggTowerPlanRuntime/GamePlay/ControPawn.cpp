// Fill out your copyright notice in the Description page of Project Settings.


#include "ControPawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TYPlayerController.h"

// Sets default values
AControPawn::AControPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootCompment = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootCompment);
	SpringArmComponent->TargetArmLength = 0.0f;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->bEnableCameraRotationLag = true;
	//SpringArmComponent->b

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	ArrowComponent =  CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComponent->SetupAttachment(RootCompment);
	
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
}

// Called when the game starts or when spawned
void AControPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AControPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AControPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
			EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &AControPawn::MoveForward);
		}

		if(IA_MoveRight)
		{
			EnhancedInputComponent->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &AControPawn::MoveRight);
		}

		if(IA_PushCamera)
		{
			EnhancedInputComponent->BindAction(IA_PushCamera, ETriggerEvent::Triggered, this, &AControPawn::PushCamera);
		}
		if (IA_OnClick)
		{
			EnhancedInputComponent->BindAction(IA_OnClick, ETriggerEvent::Started, this, &AControPawn::OnClickStarted);
			EnhancedInputComponent->BindAction(IA_OnClick, ETriggerEvent::Completed, this, &AControPawn::OnClickCompleted);

		}
		if (IA_Move)
		{
			EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AControPawn::Move);
		}
	}
}

void AControPawn::MoveForward(const FInputActionValue& InputActionValue)
{
	if (GetController()&&InputActionValue.GetMagnitude()!=0.0f)
	{
		const FRotator Ratation = GetControlRotation();
		const FRotator YawRation = FRotator(0.0f,Ratation.Yaw,0.0f);
		const FVector Dirction = FRotationMatrix(YawRation).GetUnitAxis(EAxis::X);
		AddMovementInput(Dirction,InputActionValue.GetMagnitude());
	}
}

void AControPawn::MoveRight(const FInputActionValue& InputActionValue)
{
	if (GetController()&&InputActionValue.GetMagnitude()!=0.0f)
	{
		const FRotator Ratation = GetControlRotation();
		const FRotator YawRation = FRotator(0.0f,Ratation.Yaw,0.0f);
		const FVector Dirction = FRotationMatrix(YawRation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Dirction,InputActionValue.GetMagnitude());
	}
}

void AControPawn::Move(const FInputActionValue& InputActionValue)
{
	if (GetController()&&Cast<ATYPlayerController>(GetController())&&!Cast<ATYPlayerController>(GetController())->SelectedActor)
	{
		FInputActionValue::Axis2D InputVector2D= InputActionValue.Get<FInputActionValue::Axis2D>();

		const FRotator Ratation = GetControlRotation();
		const FRotator YawRation = FRotator(0.0f,Ratation.Yaw,0.0f);
		
		const FVector VDirctionX = FRotationMatrix(YawRation).GetUnitAxis(EAxis::X)*InputVector2D.Y;
		const FVector VDirctionY = FRotationMatrix(YawRation).GetUnitAxis(EAxis::Y)*InputVector2D.X;
		const FVector Dirction = (VDirctionX+VDirctionY)*CurrentMoveSize;
		//const FVector Dirction = FVector(InputVector2D.X,InputVector2D.Y,0.0F);
		//AddMovementInput(Dirction,1.0f);
		AddActorWorldOffset(Dirction);
	}
}

void AControPawn::PushCamera(const FInputActionValue& InputActionValue)
{
	if (GetController()&&InputActionValue.GetMagnitude()!=0.0f)
	{
		if (0.0f<CameraOffsize_Current + (InputActionValue.GetMagnitude()*CameraOffsize_Rate)&&CameraOffsize_Current + (InputActionValue.GetMagnitude()*CameraOffsize_Rate)<CameraOffsize_Max)
		{
			CameraOffsize_Current +=(InputActionValue.GetMagnitude()*CameraOffsize_Rate);
			SpringArmComponent->TargetArmLength = CameraOffsize_Current;

			CurrentMoveSize = ((CameraOffsize_Max*2-CameraOffsize_Current)/CameraOffsize_Max)*BaseMoveSize;
		}
	}
}

void AControPawn::OnClickStarted(const FInputActionValue& InputActionValue)
{
	if (GetController()&&Cast<ATYPlayerController>(GetController()))
	{
		Cast<ATYPlayerController>(GetController())->OnClickStarted();
	}
}

void AControPawn::OnClickCompleted(const FInputActionValue& InputActionValue)
{
	if (GetController()&&Cast<ATYPlayerController>(GetController()))
	{
		Cast<ATYPlayerController>(GetController())->OnClickCompleted();
	}
}
