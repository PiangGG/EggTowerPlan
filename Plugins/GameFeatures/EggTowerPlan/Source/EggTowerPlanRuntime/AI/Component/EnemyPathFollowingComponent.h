// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"
#include "EnemyPathFollowingComponent.generated.h"

/**
 * 
 */
UCLASS()
class EGGTOWERPLANRUNTIME_API UEnemyPathFollowingComponent : public UPathFollowingComponent
{
	GENERATED_UCLASS_BODY()
public:
	virtual FAIRequestID RequestMove(const FAIMoveRequest& RequestData, FNavPathSharedPtr InPath) override;

	UFUNCTION(BlueprintPure)
	FVector GetDirection();

	UPROPERTY(BlueprintReadWrite)
	bool bIsRequestMove;
};


