// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularAIController.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "BaseEnemyAIController.generated.h"

/**
 * 
 */
class UEnemyPathFollowingComponent;
struct FChangeAIMessage;
UCLASS()
class EGGTOWERPLANRUNTIME_API ABaseEnemyAIController : public AModularAIController
{
	GENERATED_BODY()
public:
	ABaseEnemyAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void ReStartBehaviorTree();
	
	UFUNCTION(BlueprintCallable)
	void StopBehaviorTree();

	UFUNCTION(BlueprintCallable)
	void StartBehaviorTree();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Enemy)
	class UBehaviorTree* BehaviorTree;

	UFUNCTION(BlueprintCallable)
	void ChangeAIState(EEnemyState InAIState);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_ChangeAIState(EEnemyState InAIState);
protected:
	UPROPERTY()
	TObjectPtr<UEnemyPathFollowingComponent> EnemyPathFollowingComponent;
	UPROPERTY()
	ABaseEnemy* BaseEnemy;
public:
	FGameplayMessageListenerHandle ListenerHandle;
	
	void OnChangeAIStateMessage(FGameplayTag Channel, const FChangeAIMessage& Message);
	
};


