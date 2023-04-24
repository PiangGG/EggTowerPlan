// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "EggTowerPlanRuntime/AI/Pawn/BaseBuild.h"
#include "EggTowerPlanRuntime/AI/Pawn/BaseEnemy.h"
#include "AIManageComponent.generated.h"

/**
 * 
 */
UCLASS()
class EGGTOWERPLANRUNTIME_API UAIManageComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UAIManageComponent(const FObjectInitializer& ObjectInitializer);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	virtual void RegisterBuild(ABaseBuild* Build);

	virtual void UnRegisterBuild(ABaseBuild* Build);

	virtual void RegisterEnemy(ABaseEnemy* Enemy);

	virtual void UnRegisterEnemy(ABaseEnemy* Enemy);

	UFUNCTION(BlueprintCallable,Category=AIManage)
	virtual int32 GetEnemyNum();
	UFUNCTION(BlueprintCallable,Category=AIManage)
	virtual int32 GetBuildNum();
private:
	UPROPERTY()
	TArray<ABaseBuild*>BuildList;
	
	UPROPERTY()
	TArray<ABaseEnemy*>EnemyList;
};


