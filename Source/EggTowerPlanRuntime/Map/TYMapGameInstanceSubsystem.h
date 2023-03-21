// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TYMapGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class EGGTOWERPLANRUNTIME_API UTYMapGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UTYMapGameInstanceSubsystem();

public:

	//
	UFUNCTION(BlueprintCallable)
	void InitMap();
	//设置地图 单元显示
	UFUNCTION(BlueprintCallable)
	void SetUnitVisble(bool bVisble);
};
