// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EggTowerPlanRuntime/AI/Pawn/BaseBuild.h"
#include "CoreUnit.generated.h"

UCLASS()
class EGGTOWERPLANRUNTIME_API ACoreUnit : public ABaseBuild
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoreUnit(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	
	
	

	virtual void OnExperienceLoaded(const class ULyraExperienceDefinition* Definition)override;

	
};
