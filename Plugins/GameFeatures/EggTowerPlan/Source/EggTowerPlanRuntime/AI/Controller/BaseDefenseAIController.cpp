// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseDefenseAIController.h"
#include "NativeGameplayTags.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "EggTowerPlanRuntime/AI/Pawn/BaseBuild.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "EggTowerPlanRuntime/Tool/StructLib.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Defense_ChangeAIState, "Ai.Defense.ChangeAIState");

ABaseDefenseAIController::ABaseDefenseAIController(const FObjectInitializer& ObjectInitializer)
{
	
}

void ABaseDefenseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (IsValid(InPawn))
	{
		BaseBuild = Cast<ABaseBuild>(InPawn);
	}

	RunBehaviorTree(BehaviorTree);
	
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	ListenerHandle = MessageSubsystem.RegisterListener(TAG_Defense_ChangeAIState, this, &ThisClass::OnChangeAIStateMessage);
}

void ABaseDefenseAIController::OnUnPossess()
{
	Super::OnUnPossess();
	
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(ListenerHandle);
}

void ABaseDefenseAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ABaseDefenseAIController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ABaseDefenseAIController::OnChangeAIStateMessage(FGameplayTag Channel, const FChangeDefenseAIMessage& Message)
{
	if(Message.Controller == this)
	{
		ChangeAIState(Message.AIState);
	}
}

void ABaseDefenseAIController::ReStartBehaviorTree()
{
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComp)
	{
		BTComp->RestartTree();
	}
}

void ABaseDefenseAIController::StopBehaviorTree()
{
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComp )
	{
		BTComp->StopTree(EBTStopMode::Forced);
	}
}

void ABaseDefenseAIController::StartBehaviorTree()
{
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComp )
	{
		BTComp->RestartLogic();
	}
}

void ABaseDefenseAIController::ChangeAIState(EDefenseState InAIState)
{
	if(!IsValid(BaseBuild))
		return;
	
	BaseBuild->SetCurrentAiState(InAIState);
	
	switch (InAIState)
	{
		case EEnemyState::EIdle: break;
		case EEnemyState::EMoveToTarget: break;
		case EEnemyState::EBeHit: break;
		case EEnemyState::EUsingAbility: break;
		case EEnemyState::EAttack: break;
		case EEnemyState::ENULL: break;
		default: ;
	}
	K2_ChangeAIState(InAIState);
}
