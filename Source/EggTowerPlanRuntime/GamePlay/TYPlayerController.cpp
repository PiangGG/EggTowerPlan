// Fill out your copyright notice in the Description page of Project Settings.


#include "TYPlayerController.h"

#include "EggTowerPlanRuntime/Attacker/BaseAttacker.h"
#include "EggTowerPlanRuntime/Map/MapUnit.h"
#include "EggTowerPlanRuntime/Tool/EnumLib.h"
#include "EggTowerPlanRuntime/Tool/ToolLibrary.h"
#include "Kismet/KismetSystemLibrary.h"



ATYPlayerController::ATYPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
}

void ATYPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateSelectedActor(DeltaSeconds);
}

void ATYPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATYPlayerController::SetControllerState(EControllerState State)
{
	ControllerState=State;

	switch (ControllerState)
	{
		case EControllerState::EBuild:
			
			break;
		
		case EControllerState::ECombat:
			
			break;
		default: ;
	}
}

EControllerState ATYPlayerController::GetControllerState()
{
	return ControllerState;
}

void ATYPlayerController::UpdateSelectedActor(float DeltaSeconds)
{
	//Tick选中Actor的位置
	if (ControllerState==EControllerState::EBuild&&SelectedActor&&GetWorld())
	{
		FHitResult HitResult;
		FVector StartLocation,Direction,EndLocation;;
		DeprojectMousePositionToWorld(StartLocation,Direction);
		EndLocation = StartLocation+(Direction*LineLength);

		const TArray<AActor*> ActorsToIgnore;
		if (UKismetSystemLibrary::LineTraceSingle(GetWorld(),StartLocation,EndLocation,ETraceTypeQuery::TraceTypeQuery3,true,ActorsToIgnore,EDrawDebugTrace::ForOneFrame,HitResult,true))
		{
			if (HitResult.GetActor()&&Cast<AMapUnit>(HitResult.GetActor()))
			{
				SelectedActor->SetActorLocation(Cast<AMapUnit>(HitResult.GetActor())->GetBuildLocation());
				// LastHoverMapUnit = CurrentHoverMapUnit;
				// CurrentHoverMapUnit = Cast<AMapUnit>(HitResult.GetActor());
				return;
			}
		}
	}
	else if (ControllerState==EControllerState::EBuild&&!SelectedActor&&GetWorld())
	{
		// LastHoverMapUnit = CurrentHoverMapUnit;
		// CurrentHoverMapUnit = nullptr;
	}
}

void ATYPlayerController::OnClick(AActor* TargetActor)
{
	switch (ControllerState)
	{
	case EControllerState::EBuild:
		SetSelectedActor(TargetActor);
		break;
	case EControllerState::ECombat:
		SelectedActorAttack(TargetActor);
		break;
	default: ;
	}
}

bool ATYPlayerController::GetOnClickbMapUnitActor()
{
	FHitResult HitResult;
	FVector StartLocation,Direction,EndLocation;;
	DeprojectMousePositionToWorld(StartLocation,Direction);
	EndLocation = StartLocation+(Direction*LineLength);

	const TArray<AActor*> ActorsToIgnore;
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(),StartLocation,EndLocation,ETraceTypeQuery::TraceTypeQuery3,true,ActorsToIgnore,EDrawDebugTrace::ForOneFrame,HitResult,true))
	{
		if (HitResult.GetActor()&&Cast<AMapUnit>(HitResult.GetActor()))
		{
			return true;
		}
	}
	return  false;
}


void ATYPlayerController::SetSelectedActor(AActor* TargetActor)
{
	check(TargetActor)
	if (!SelectedActor)
	{
		SelectedActor = TargetActor;
		Cast<ABaseAttacker>(TargetActor)->OnSelected();
		UToolLibrary::DebugLog("SetNewSelectedActor");
	}
	else
	{
		SelectedActor = nullptr;
		SetActorLocationToUnitMap(TargetActor);
		Cast<ABaseAttacker>(TargetActor)->UnSelected();
		UToolLibrary::DebugLog("SetNullSelectedActor");
	}
}

void ATYPlayerController::SelectedActorAttack(AActor* TargetActor)
{
	if (Cast<ABaseAttacker>(TargetActor))
	{
		Cast<ABaseAttacker>(TargetActor)->Attack();
	}
}

void ATYPlayerController::SetActorLocationToUnitMap(AActor* TargetActor)
{
	if (TargetActor&&GetWorld())
	{
		// FHitResult HitResult;
		// FVector StartLocation,Direction,EndLocation;;
		// DeprojectMousePositionToWorld(StartLocation,Direction);
		// EndLocation = StartLocation+(Direction*LineLength);
		//
		// const TArray<AActor*> ActorsToIgnore;
		// if (UKismetSystemLibrary::LineTraceSingle(GetWorld(),StartLocation,EndLocation,ETraceTypeQuery::TraceTypeQuery3,true,ActorsToIgnore,EDrawDebugTrace::ForDuration,HitResult,true,FLinearColor::Blue))
		// {
		// 	if (HitResult.GetActor()&&Cast<AMapUnit>(HitResult.GetActor()))
		// 	{
		// 		if (Cast<ABaseAttacker>(TargetActor)->CanSetActorLocation())
		// 		{
		// 			TargetActor->SetActorLocation(Cast<AMapUnit>(HitResult.GetActor())->GetBuildLocation());
		// 			Cast<ABaseAttacker>(TargetActor)->AtMapUnit = Cast<AMapUnit>(HitResult.GetActor());
		// 		}
		// 		else
		// 		{
		// 			if (Cast<ABaseAttacker>(TargetActor)->AtMapUnit)
		// 			{
		// 				TargetActor->SetActorLocation(Cast<ABaseAttacker>(TargetActor)->AtMapUnit->GetBuildLocation());
		// 			}
		// 			else
		// 			{
		// 				TargetActor->SetActorLocation(FVector::ZeroVector);
		// 				UToolLibrary::DebugLog("TargetActor no AtMapUnit last");
		// 			}
		// 		}
		// 	}
		// }
		if (SelectedActor&&TargetActor&&Cast<AMapUnit>(TargetActor))
		{
			if (Cast<ABaseAttacker>(SelectedActor)->CanSetActorLocation())
			{
				SelectedActor->SetActorLocation(Cast<AMapUnit>(TargetActor)->GetBuildLocation());
				Cast<ABaseAttacker>(SelectedActor)->AtMapUnit = Cast<AMapUnit>(TargetActor);
			}
			else
			{
				if (Cast<ABaseAttacker>(SelectedActor)->AtMapUnit)
				{
					SelectedActor->SetActorLocation(Cast<ABaseAttacker>(SelectedActor)->AtMapUnit->GetBuildLocation());
				}
				else
				{
					SelectedActor->SetActorLocation(FVector::ZeroVector);
					UToolLibrary::DebugLog("TargetActor no AtMapUnit last");
				}
			}
		}
	}
}

void ATYPlayerController::LineTraceMapUnit()
{
	UToolLibrary::DebugLog("LineTraceMapUnit");
}

void ATYPlayerController::OnClickStarted()
{
	UToolLibrary::DebugLog("OnClickStarted");

	bOnCilcking = true;
	
	if (ControllerState ==EControllerState::EBuild)
	{
		FHitResult HitResult;
		FVector StartLocation,Direction,EndLocation;;
		DeprojectMousePositionToWorld(StartLocation,Direction);
		EndLocation = StartLocation+(Direction*LineLength);

		const TArray<AActor*> ActorsToIgnore;
		if (UKismetSystemLibrary::LineTraceSingle(GetWorld(),StartLocation,EndLocation,ETraceTypeQuery::TraceTypeQuery6,true,ActorsToIgnore,EDrawDebugTrace::ForDuration,HitResult,true,FLinearColor::Red))
		{
			if (HitResult.GetActor()&&Cast<ABaseAttacker>(HitResult.GetActor()))
			{
				if (!SelectedActor)
				{
					SelectedActor = HitResult.GetActor();
					Cast<ABaseAttacker>(HitResult.GetActor())->OnSelected();
					UToolLibrary::DebugLog("SetNewSelectedActor");
				}
			}
		}
	}
	if (ControllerState ==EControllerState::ECombat)
	{
		FHitResult HitResult;
		FVector StartLocation,Direction,EndLocation;;
		DeprojectMousePositionToWorld(StartLocation,Direction);
		EndLocation = StartLocation+(Direction*LineLength);

		const TArray<AActor*> ActorsToIgnore;
		if (UKismetSystemLibrary::LineTraceSingle(GetWorld(),StartLocation,EndLocation,ETraceTypeQuery::TraceTypeQuery6,true,ActorsToIgnore,EDrawDebugTrace::ForDuration,HitResult,true,FLinearColor::Red))
		{
			if (HitResult.GetActor()&&Cast<ABaseAttacker>(HitResult.GetActor()))
			{
				Cast<ABaseAttacker>(HitResult.GetActor())->Attack();
				UToolLibrary::DebugLog("OnClick");
			}
		}
	}
}

void ATYPlayerController::OnClickCompleted()
{
	UToolLibrary::DebugLog("OnClickCompleted");

	if (ControllerState ==EControllerState::EBuild)
	{
		FHitResult HitResult;
		FVector StartLocation,Direction,EndLocation;;
		DeprojectMousePositionToWorld(StartLocation,Direction);
		EndLocation = StartLocation+(Direction*LineLength);

		const TArray<AActor*> ActorsToIgnore;
		if (UKismetSystemLibrary::LineTraceSingle(GetWorld(),StartLocation,EndLocation,ETraceTypeQuery::TraceTypeQuery3,true,ActorsToIgnore,EDrawDebugTrace::ForDuration,HitResult,true,FLinearColor::Green))
		{
			if (HitResult.GetActor()&&Cast<AMapUnit>(HitResult.GetActor())&&SelectedActor)
			{
				SetActorLocationToUnitMap(HitResult.GetActor());
				Cast<ABaseAttacker>(SelectedActor)->UnSelected();
				SelectedActor = nullptr;
			}
		}
	}
	bOnCilcking = false;
}
