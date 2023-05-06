// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GameProcessManageComponent.generated.h"

class ULyraExperienceDefinition;
/**
 * 
 */
UCLASS()
class EGGTOWERPLANRUNTIME_API UGameProcessManageComponent : public UGameStateComponent
{
	GENERATED_BODY()
public:
	UGameProcessManageComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	
	
	void OnExperienceLoaded(const ULyraExperienceDefinition*);

	UFUNCTION(BlueprintNativeEvent)
	void IntoMapStartGame();
	
};
