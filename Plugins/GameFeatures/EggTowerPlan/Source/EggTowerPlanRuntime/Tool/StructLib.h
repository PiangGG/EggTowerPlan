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
USTRUCT(BlueprintType)
struct FChangeAIMessage
{
	GENERATED_BODY()

	//接收的controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AController* Controller = nullptr;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemyState AIState = EEnemyState::EIdle;
};

USTRUCT(BlueprintType)
struct FChangeCoreUnitMessage
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECoreUnitChange CoreUnitChange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACoreUnit* CoreUnit;
};

class EGGTOWERPLANRUNTIME_API StructLib
{
public:
	StructLib();
	~StructLib();
};
