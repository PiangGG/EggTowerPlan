// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_BaseEnemy.h"

#include "BehaviorTree/BlackboardComponent.h"

void UBTService_BaseEnemy::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UBTService_BaseEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	EnemyAIController = Cast<ABaseEnemyAIController>(OwnerComp.GetAIOwner());

	if (EnemyAIController)
	{
		BaseEnemy = Cast<ABaseEnemy>(EnemyAIController->GetPawn());
	}
	if (!IsValid(EnemyAIController)||!IsValid(BaseEnemy))
	{
		return;
	}

	if (AIState!=NAME_None)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AIState,(uint8)BaseEnemy->GetCurrentAiState());
	}

	if (TargetActor!=NAME_None)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetActor,BaseEnemy->GetTargetActor());
	}

	if (TargetLocation!=NAME_None)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocation,BaseEnemy->GetTargetMoveLocation());
	}
	
	if (TargetLastLocation!=NAME_None)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLastLocation,BaseEnemy->GetTargetLastMoveLocation());
	}
}
