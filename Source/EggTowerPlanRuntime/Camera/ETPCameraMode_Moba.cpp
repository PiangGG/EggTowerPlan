// Fill out your copyright notice in the Description page of Project Settings.


#include "ETPCameraMode_Moba.h"

#include "Components/CapsuleComponent.h"
#include "EggTowerPlanRuntime/Component/ETPHeroComponent.h"
#include "GameFramework/Character.h"

UETPCameraMode_Moba::UETPCameraMode_Moba()
{
	
}

void UETPCameraMode_Moba::UpdateView(float DeltaTime)
{
	FVector PivotLocation = GetPivotLocation() + DefaultPivotLocation;
	FRotator PivotRotation = DefaultPivotRotation;

	if (bOpenBlend)
	{
		SetLastFrameLocation(FMath::Lerp(GetLastFrameLocation(), PivotLocation, DeltaTime * BlendSpeed));
		SetLastFrameRotation( FMath::Lerp(GetLastFrameRotation(), PivotRotation, DeltaTime * BlendSpeed));
		
		View.Location = GetLastFrameLocation();
		View.Rotation = GetLastFrameRotation();
		
		View.ControlRotation = View.Rotation;
		View.FieldOfView = FieldOfView;
	}
	else
	{
		View.Location = PivotLocation;
		View.Rotation = PivotRotation;
		
		View.ControlRotation = View.Rotation;
		View.FieldOfView = FieldOfView;
	}
}

FVector UETPCameraMode_Moba::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// Height adjustments for characters to account for crouching.
		if (const ACharacter* TargetCharacter = Cast<ACharacter>(TargetPawn))
		{
			const ACharacter* TargetCharacterCDO = TargetCharacter->GetClass()->GetDefaultObject<ACharacter>();
			check(TargetCharacterCDO);

			const UCapsuleComponent* CapsuleComp = TargetCharacter->GetCapsuleComponent();
			check(CapsuleComp);

			const UCapsuleComponent* CapsuleCompCDO = TargetCharacterCDO->GetCapsuleComponent();
			check(CapsuleCompCDO);

			const float DefaultHalfHeight = CapsuleCompCDO->GetUnscaledCapsuleHalfHeight();
			const float ActualHalfHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
			const float HeightAdjustment = (DefaultHalfHeight - ActualHalfHeight) + TargetCharacterCDO->BaseEyeHeight;

			return TargetCharacter->GetActorLocation() + (FVector::UpVector * HeightAdjustment);
		}

		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FVector UETPCameraMode_Moba::GetLastFrameLocation()
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (UETPHeroComponent * HeroComponent = Cast<UETPHeroComponent>(GetTargetActor()->GetComponentByClass(UETPHeroComponent::StaticClass())))
	{
		return HeroComponent->LastFrameLocation;
	}
	return FVector::ZeroVector;
}

void UETPCameraMode_Moba::SetLastFrameLocation(FVector Vector)
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (UETPHeroComponent * HeroComponent = Cast<UETPHeroComponent>(GetTargetActor()->GetComponentByClass(UETPHeroComponent::StaticClass())))
	{
		HeroComponent->SetLastFrameLocation(Vector);
	}
}

FRotator UETPCameraMode_Moba::GetLastFrameRotation()
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (UETPHeroComponent * HeroComponent = Cast<UETPHeroComponent>(GetTargetActor()->GetComponentByClass(UETPHeroComponent::StaticClass())))
	{
		return HeroComponent->LastFrameRotation;
	}
	return FRotator::ZeroRotator;
}

void UETPCameraMode_Moba::SetLastFrameRotation(FRotator Rotator)
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (UETPHeroComponent * HeroComponent = Cast<UETPHeroComponent>(GetTargetActor()->GetComponentByClass(UETPHeroComponent::StaticClass())))
	{
		HeroComponent->SetLastFrameRotation(Rotator);
	}
}
