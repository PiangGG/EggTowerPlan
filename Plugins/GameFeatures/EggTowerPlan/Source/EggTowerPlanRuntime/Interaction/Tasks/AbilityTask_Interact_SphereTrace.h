// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractionQuery.h"
#include "Interaction/Tasks/AbilityTask_WaitForInteractableTargets.h"
#include "AbilityTask_Interact_SphereTrace.generated.h"

class UGameplayAbility;
class UObject;
struct FFrame;
/**
 * 
 */
UCLASS()
class EGGTOWERPLANRUNTIME_API UAbilityTask_Interact_SphereTrace : public UAbilityTask_WaitForInteractableTargets
{
	GENERATED_UCLASS_BODY()
	
	virtual void Activate() override;

	/** Wait until we trace new set of interactables.  This task automatically loops. */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_Interact_SphereTrace* WaitForInteractableTargets_SphereTrace(UGameplayAbility* OwningAbility, FInteractionQuery InteractionQuery, FCollisionProfileName TraceProfile, FGameplayAbilityTargetingLocationInfo StartLocation, float InteractionScanRange = 100, float InteractionScanRate = 0.100, bool bShowDebug = false);

	static void SphereTrace(TArray<FOverlapResult>& OverlapResults,TArray<TScriptInterface<IInteractableTarget>>& InteractableTargetsRef,FHitResult& OutHitResult,const UWorld* World, const FVector& Start,float Radius, FName ProfileName, const FCollisionQueryParams Params);

private:

	virtual void OnDestroy(bool AbilityEnded) override;

	void PerformTrace();

	UPROPERTY()
	FInteractionQuery InteractionQuery;

	UPROPERTY()
	FGameplayAbilityTargetingLocationInfo StartLocation;

	float InteractionScanRange = 100;
	float InteractionScanRate = 0.100;
	bool bShowDebug = false;

	FTimerHandle TimerHandle;
};
