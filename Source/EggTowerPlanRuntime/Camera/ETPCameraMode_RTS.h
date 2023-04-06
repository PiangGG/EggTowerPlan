// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/LyraCameraMode.h"
#include "ETPCameraMode_RTS.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class EGGTOWERPLANRUNTIME_API UETPCameraMode_RTS : public ULyraCameraMode
{
	GENERATED_BODY()
	
	UETPCameraMode_RTS();

protected:

	//~ULyraCameraMode interface
	virtual void UpdateView(float DeltaTime) override;
	//~End of ULyraCameraMode interface

	void UpdateForTarget(float DeltaTime);
	void UpdatePreventPenetration(float DeltaTime);
	void PreventCameraPenetration(class AActor const& ViewTarget, FVector const& SafeLoc, FVector& CameraLoc, float const& DeltaTime, float& DistBlockedPct, bool bSingleRayOnly);

	UPROPERTY(EditDefaultsOnly, Category = "RTS|CurentRotation")
	FRotator DefaultPivotRotation;

	//位置偏移
	UPROPERTY(EditDefaultsOnly, Category = "RTS|OffsetLocation")
	FVector DefaultPivotLocation = FVector::ZeroVector;

	//到地面的距离
	UPROPERTY(EditDefaultsOnly, Category = "RTS|距离地面距离")
	FRuntimeFloatCurve GroundToDistance;

	virtual FVector GetPivotLocation() const override;
	
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Blend")
	bool bOpenBlend = true;
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Blend")
	float BlendSpeed = 1.0f;
	
	FVector GetLastFrameLocation();
	void SetLastFrameLocation(FVector Vector);
	FRotator GetLastFrameRotation();
	void SetLastFrameRotation(FRotator Rotator);
};
