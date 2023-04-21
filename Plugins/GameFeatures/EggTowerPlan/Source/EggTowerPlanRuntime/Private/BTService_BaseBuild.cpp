// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_BaseBuild.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EggTowerPlanRuntime/AI/Controller/BaseDefenseAIController.h"

void UBTService_BaseBuild::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UBTService_BaseBuild::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	DefenseAIController = Cast<ABaseDefenseAIController>(OwnerComp.GetAIOwner());

	if (DefenseAIController)
	{
		BaseBuild = Cast<ABaseBuild>(DefenseAIController->GetPawn());
	}
	if (!IsValid(DefenseAIController)||!IsValid(BaseBuild))
	{
		return;
	}

	if (AIState!=NAME_None)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AIState,(uint8)BaseBuild->GetCurrentAiState());
	}

	if (TargetActor!=NAME_None)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetActor,BaseBuild->GetTargetActor());
	}
}
