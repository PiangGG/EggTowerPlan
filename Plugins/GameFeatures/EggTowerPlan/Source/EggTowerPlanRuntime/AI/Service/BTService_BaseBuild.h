// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "EggTowerPlanRuntime/AI/Pawn/BaseBuild.h"
#include "BTService_BaseBuild.generated.h"

/**
 * 
 */
class ABaseDefenseAIController;
UCLASS()
class EGGTOWERPLANRUNTIME_API UBTService_BaseBuild : public UBTService
{
	GENERATED_BODY()
public:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	ABaseDefenseAIController* DefenseAIController;
	
	UPROPERTY(BlueprintReadOnly)
	ABaseBuild *BaseBuild;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=EnemyService)
	FName TargetActor = TEXT("TargetActor");

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=EnemyService)
	FName TargetLocation = TEXT("TargetLocation");
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=EnemyService)
	FName AIState = TEXT("AIState");
};
