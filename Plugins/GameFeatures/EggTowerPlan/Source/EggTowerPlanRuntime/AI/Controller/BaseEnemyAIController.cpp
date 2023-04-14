// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAIController.h"
#include "NativeGameplayTags.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "EggTowerPlanRuntime/AI/Component/EnemyPathFollowingComponent.h"
#include "EggTowerPlanRuntime/AI/Pawn/BaseEnemy.h"
#include "EggTowerPlanRuntime/Tool/StructLib.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Enemy_ChangeAIState, "Ai.Enemy.ChangeAIState");

ABaseEnemyAIController::ABaseEnemyAIController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UEnemyPathFollowingComponent>(TEXT("PathFollowingComponent")))
{
	
}

void ABaseEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (IsValid(InPawn))
	{
		BaseEnemy = Cast<ABaseEnemy>(InPawn);
	}

	RunBehaviorTree(BehaviorTree);
	
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	ListenerHandle = MessageSubsystem.RegisterListener(TAG_Enemy_ChangeAIState, this, &ThisClass::OnChangeAIStateMessage);
}

void ABaseEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
	
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(ListenerHandle);
}

void ABaseEnemyAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ABaseEnemyAIController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ABaseEnemyAIController::OnChangeAIStateMessage(FGameplayTag Channel, const FChangeAIMessage& Message)
{
	if(Message.Controller == this)
	{
		ChangeAIState(Message.AIState);
	}
}

void ABaseEnemyAIController::ReStartBehaviorTree()
{
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComp)
	{
		BTComp->RestartTree();
	}
}

void ABaseEnemyAIController::StopBehaviorTree()
{
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComp )
	{
		BTComp->StopTree(EBTStopMode::Forced);
	}
}

void ABaseEnemyAIController::StartBehaviorTree()
{
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComp )
	{
		BTComp->RestartLogic();
	}
}

void ABaseEnemyAIController::ChangeAIState(EEnemyState InAIState)
{
	if(!IsValid(BaseEnemy))
		return;
	
	BaseEnemy->SetCurrentAiState(InAIState);
	
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
