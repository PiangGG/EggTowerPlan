// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularAIController.h"
#include "EggTowerPlanRuntime/Tool/EnumLib.h"
#include "EggTowerPlanRuntime/Tool/StructLib.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "BaseDefenseAIController.generated.h"

/**
 * 
 */
class ABaseBuild;
UCLASS()
class EGGTOWERPLANRUNTIME_API ABaseDefenseAIController : public AModularAIController
{
	GENERATED_BODY()
public:
	ABaseDefenseAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

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
	void ChangeAIState(EDefenseState InAIState);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_ChangeAIState(EDefenseState InAIState);

protected:
	UPROPERTY()
	ABaseBuild* BaseBuild;
public:
	FGameplayMessageListenerHandle ListenerHandle;
	
	void OnChangeAIStateMessage(FGameplayTag Channel, const FChangeDefenseAIMessage& Message);
};
