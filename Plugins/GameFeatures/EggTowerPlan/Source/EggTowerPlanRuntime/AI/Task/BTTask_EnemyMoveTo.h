// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_EnemyMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class EGGTOWERPLANRUNTIME_API UBTTask_EnemyMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere,Category="EnemyMove")
	FTimerHandle TimerHandle_MoveCheak;

	UPROPERTY(EditAnywhere,Category="EnemyMove")
	float MoveCheckTiem = 3.f;

	void UpdateMoveCheak(EBTNodeResult::Type &Type);
};
