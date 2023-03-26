// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumLib.h"
#include "StructLib.generated.h"
/**
 * 
 */

USTRUCT(BlueprintType)
struct EGGTOWERPLANRUNTIME_API FPCModeChange
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= PC)
	EControllerState PCState;
};

class EGGTOWERPLANRUNTIME_API StructLib
{
public:
	StructLib();
	~StructLib();
};
