// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/LyraCameraMode.h"
#include "ETPCameraMode_Moba.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class EGGTOWERPLANRUNTIME_API UETPCameraMode_Moba : public ULyraCameraMode
{
	GENERATED_BODY()
public:

	UETPCameraMode_Moba();

protected:

	//~ULyraCameraMode interface
	virtual void UpdateView(float DeltaTime) override;
	//~End of ULyraCameraMode interface

	UPROPERTY(EditDefaultsOnly, Category = "Moba|CurentRotation")
	FRotator DefaultPivotRotation;

	//位置偏移
	UPROPERTY(EditDefaultsOnly, Category = "Moba|OffsetLocation")
	FVector DefaultPivotLocation = FVector::ZeroVector;

	//到地面的距离
	UPROPERTY(EditDefaultsOnly, Category = "Moba|")
	FRuntimeFloatCurve GroundToDistance;

	virtual FVector GetPivotLocation() const override;
};
