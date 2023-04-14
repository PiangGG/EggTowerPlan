// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "CoreUnitManageComponent.generated.h"

/**
 * 
 */
struct FChangeCoreUnitMessage;
UCLASS()
class EGGTOWERPLANRUNTIME_API UCoreUnitManageComponent : public UGameStateComponent
{
	GENERATED_BODY()
public:
	UCoreUnitManageComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void OnChangeCoreUnitMessage(FGameplayTag Channel, const FChangeCoreUnitMessage& Message);
	
	FGameplayMessageListenerHandle ListenerHandle;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=CoreUnit)
	TArray<ACoreUnit*> CoreUnit;
};
