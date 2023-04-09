// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_Interact_SphereTrace.h"

#include "Interaction/InteractionStatics.h"

UAbilityTask_Interact_SphereTrace::UAbilityTask_Interact_SphereTrace(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UAbilityTask_Interact_SphereTrace::Activate()
{
	SetWaitingOnAvatar();

	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::PerformTrace, InteractionScanRate, true);
}

UAbilityTask_Interact_SphereTrace* UAbilityTask_Interact_SphereTrace::WaitForInteractableTargets_SphereTrace(UGameplayAbility* OwningAbility,
	FInteractionQuery InteractionQuery, FCollisionProfileName TraceProfile,
	FGameplayAbilityTargetingLocationInfo StartLocation, float InteractionScanRange, float InteractionScanRate,
	bool bShowDebug)
{
	UAbilityTask_Interact_SphereTrace* MyObj = NewAbilityTask<UAbilityTask_Interact_SphereTrace>(OwningAbility);
	MyObj->InteractionScanRange = InteractionScanRange;
	MyObj->InteractionScanRate = InteractionScanRate;
	MyObj->StartLocation = StartLocation;
	MyObj->InteractionQuery = InteractionQuery;
	MyObj->TraceProfile = TraceProfile;
	MyObj->bShowDebug = bShowDebug;
	
	return MyObj;
}

void UAbilityTask_Interact_SphereTrace::SphereTrace(TArray<FOverlapResult>& OverlapResults,TArray<TScriptInterface<IInteractableTarget>>& InteractableTargetsRef,
	FHitResult& OutHitResult,const UWorld* World, const FVector& Start,float Radius, FName ProfileName,
	const FCollisionQueryParams Params)
{
	check(World);

	TArray<FOverlapResult> TempOverlapResults;
	World->OverlapMultiByProfile(OUT TempOverlapResults, Start, FQuat::Identity, ProfileName, FCollisionShape::MakeSphere(Radius), Params);
	
	if(TempOverlapResults.Num() > 0)
	{
		OverlapResults = TempOverlapResults;
	}
	TArray<FHitResult>HitResults;
	
	for (auto OverlapResult : OverlapResults)
	{
		TScriptInterface<IInteractableTarget> InteractableActor(OverlapResult.GetActor());
		if (InteractableActor)
		{
			FHitResult TempOutHitResult;
			LineTrace(TempOutHitResult, World, Start, OverlapResult.GetActor()->GetActorLocation(), ProfileName, Params);
			UInteractionStatics::AppendInteractableTargetsFromHitResult(TempOutHitResult, InteractableTargetsRef);
			HitResults.Add(TempOutHitResult);
		}
	}
	if (HitResults.Num()>0)
	{
		OutHitResult = HitResults[0];
		for (auto Results : HitResults)
		{
			if (OutHitResult.Distance>Results.Distance)
			{
				OutHitResult = Results;
			}
		}
	}
}

void UAbilityTask_Interact_SphereTrace::OnDestroy(bool AbilityEnded)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}

	Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_Interact_SphereTrace::PerformTrace()
{
	AActor* AvatarActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
	if (!AvatarActor)
	{
		return;
	}

	UWorld* World = GetWorld();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AvatarActor);

	const bool bTraceComplex = false;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(UAbilityTask_WaitForInteractableTargets_SingleLineTrace), bTraceComplex);
	Params.AddIgnoredActors(ActorsToIgnore);

	FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();
	
	TArray<FOverlapResult> OverlapResults;

	TArray<TScriptInterface<IInteractableTarget>> InteractableTargets;
	FHitResult OutHitResult;
	SphereTrace(OverlapResults,InteractableTargets,OutHitResult, World, TraceStart,InteractionScanRange,TraceProfile.Name, Params);

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FColor DebugColor = InteractableTargets.Num()>0 ? FColor::Red : FColor::Green;
		if (InteractableTargets.Num()>0)
		{
			DrawDebugSphere(World, OutHitResult.Location, 5, 16, DebugColor, false, InteractionScanRate);
			DrawDebugSphere(World,TraceStart, InteractionScanRange, 16, DebugColor, false, InteractionScanRate);
		}
		else
		{
			UpdateInteractableOptions(InteractionQuery, InteractableTargets);DrawDebugSphere(World, TraceStart, InteractionScanRange, 16, DebugColor, false, InteractionScanRate);
		}
	}
#endif // ENABLE_DRAW_DEBUG
}
