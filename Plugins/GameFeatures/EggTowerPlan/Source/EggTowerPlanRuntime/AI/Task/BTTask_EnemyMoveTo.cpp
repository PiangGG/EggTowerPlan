// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EnemyMoveTo.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_EnemyMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;

	FBTMoveToTaskMemory* MyMemory = CastInstanceNodeMemory<FBTMoveToTaskMemory>(NodeMemory);
	MyMemory->PreviousGoalLocation = FAISystem::InvalidLocation;
	MyMemory->MoveRequestID = FAIRequestID::InvalidRequest;

	AAIController* MyController = OwnerComp.GetAIOwner();
	if (MyController == nullptr)
	{
		//UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UBTTask_MoveTo::ExecuteTask failed since AIController is missing."));
		NodeResult = EBTNodeResult::Failed;
	}
	else
	{
		NodeResult = PerformMoveTask(OwnerComp, NodeMemory);
	}

	// FTimerDelegate UpdateAttackDelegate = FTimerDelegate::CreateUObject(this, &UBTTask_EnemyMoveTo::UpdateMoveCheak,NodeResult);
	//
	// GetWorld()->GetTimerManager().SetTimer(TimerHandle_MoveCheak,UpdateAttackDelegate,0.1f,true);
	
	if (NodeResult == EBTNodeResult::InProgress && bObserveBlackboardValue)
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		if (ensure(BlackboardComp))
		{
			if (MyMemory->BBObserverDelegateHandle.IsValid())
			{
				//UE_VLOG(MyController, LogBehaviorTree, Warning, TEXT("UBTTask_MoveTo::ExecuteTask \'%s\' Old BBObserverDelegateHandle is still valid! Removing old Observer."), *GetNodeName());
				BlackboardComp->UnregisterObserver(BlackboardKey.GetSelectedKeyID(), MyMemory->BBObserverDelegateHandle);
			}
			MyMemory->BBObserverDelegateHandle = BlackboardComp->RegisterObserver(BlackboardKey.GetSelectedKeyID(), this, FOnBlackboardChangeNotification::CreateUObject(this, &UBTTask_MoveTo::OnBlackboardValueChange));
		}
	}
	return NodeResult;
}

void UBTTask_EnemyMoveTo::UpdateMoveCheak(EBTNodeResult::Type &Type)
{
	Type = EBTNodeResult::Failed;
}
