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
	ECombat,
	ENULL
};

//Enemy状态枚举
UENUM(BlueprintType)
enum class EEnemyState :uint8
{
	EIdle,
	EMoveToTarget,
	EBeHit,
	EUsingAbility,
	EAttack,
	ENULL
};
//防御塔状态枚举
UENUM(BlueprintType)
enum class EDefenseState :uint8
{
	EIdle,
	EBeHit,
	EUsingAbility,
	EAttack,
	ENULL
};

UENUM(BlueprintType)
enum class ECoreUnitChange :uint8
{
	ECoreUnitAdd,
	ECoreUnitRemove,
	ENULL
};