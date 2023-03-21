// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class EGGTOWERPLANRUNTIME_API EnumLib
{
public:
	EnumLib();
	~EnumLib();
};

//控制器状态枚举
UENUM(BlueprintType)
enum class EControllerState :uint8
{
	EBuild,
	ECombat
};