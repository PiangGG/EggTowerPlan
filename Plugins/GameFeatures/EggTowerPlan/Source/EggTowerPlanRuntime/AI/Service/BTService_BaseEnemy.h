// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "EggTowerPlanRuntime/AI/Controller/BaseEnemyAIController.h"
#include "EggTowerPlanRuntime/AI/Pawn/BaseEnemy.h"
#include "BTService_BaseEnemy.generated.h"

/**
 * 
 */
UCLASS()
class EGGTOWERPLANRUNTIME_API UBTService_BaseEnemy : public UBTService
{
	GENERATED_BODY()
public:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	ABaseEnemyAIController* EnemyAIController;
	
	UPROPERTY(BlueprintReadOnly)
	ABaseEnemy *BaseEnemy;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=EnemyService)
	FName TargetActor = TEXT("TargetActor");

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=EnemyService)
	FName TargetLocation = TEXT("TargetLocation");

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=EnemyService)
	FName TargetLastLocation = TEXT("TargetLastLocation");

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=EnemyService)
	FName AIState = TEXT("AIState");
};
