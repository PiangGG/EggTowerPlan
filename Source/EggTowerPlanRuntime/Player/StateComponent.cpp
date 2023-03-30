// Fill out your copyright notice in the Description page of Project Settings.


#include "StateComponent.h"

#include "EggTowerPlanRuntime/Character/ETPCharacter.h"
#include "EggTowerPlanRuntime/Tool/EnumLib.h"
#include "EggTowerPlanRuntime/Tool/ToolLibrary.h"

UStateComponent::UStateComponent(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
	State = EControllerState::EBuild;
}

EControllerState UStateComponent::GetControllerState()
{
	return State;
}

void UStateComponent::SetControllerState(EControllerState ControllerState)
{
	/*if (GetOwner())
	{
		if (Cast<APlayerController>(GetOwner()))
		{
			APawn* ControedPawn = Cast<APlayerController>(GetOwner())->GetPawn();
			if (ControedPawn->GetClass()==Character_Moba_Class&&!Character_Moba)
			{
				Character_Moba = Cast<AETPCharacter>(ControedPawn);
			}

			if (ControedPawn->GetClass()==Character_RTS_Class&&!Character_RTS)
			{
				Character_RTS = Cast<AETPCharacter>(ControedPawn);
			}
			
		}
		else
		{
			UToolLibrary::DebugLog("UStateComponent 的拥有者不是AplayerController");
		}
	}
	
	switch (ControllerState)
	{
		case EControllerState::EBuild:
			ChangeStateRTS();
			break;
		case EControllerState::ECombat:
			ChangeStateMoba();
			break;
		case EControllerState::ENULL:
			
			break;
		default:
			break;
	}*/
	State = ControllerState;
}

void UStateComponent::ChangeStateMoba()
{
	if (Character_Moba&&GetOwner())
	{
		if (Cast<APlayerController>(GetOwner()))
		{
			Cast<APlayerController>(GetOwner())->Possess(Cast<APawn>(Character_Moba));
		}
		else
		{
			UToolLibrary::DebugLog("UStateComponent 的拥有者不是AplayerController");
		}
	}
	else
	{
		if (Character_Moba_Class&&GetOwner())
		{
			FVector SpawnLocation = Cast<APlayerController>(GetOwner())->GetPawn()->GetActorLocation();
			FRotator SpawnRotation = Cast<APlayerController>(GetOwner())->GetPawn()->GetActorRotation();
			FActorSpawnParameters ActorSpawnParameters;

			Character_Moba = GetWorld()->SpawnActor<AETPCharacter>(Character_Moba_Class,SpawnLocation,SpawnRotation,ActorSpawnParameters);
			
			if (Cast<APlayerController>(GetOwner())&&Character_Moba)
			{
				Cast<APlayerController>(GetOwner())->Possess(Cast<APawn>(Character_Moba));
			}else
			{
				UToolLibrary::DebugLog("UStateComponent 的拥有者不是AplayerController");
			}
		}
		else
		{
			UToolLibrary::DebugLog("Character_RTS_Class or GetOwner() 为 NULL");
		}
	}
}

void UStateComponent::ChangeStateRTS()
{
	if (Character_RTS&&GetOwner())
	{
		if (Cast<APlayerController>(GetOwner()))
		{
			Cast<APlayerController>(GetOwner())->Possess(Cast<APawn>(Character_RTS));
		}
		else
		{
			UToolLibrary::DebugLog("UStateComponent 的拥有者不是AplayerController");
		}
	}
	else
	{
		if (Character_RTS_Class&&GetOwner())
		{
			FVector SpawnLocation =Cast<APlayerController>(GetOwner())->GetPawn()->GetActorLocation();
			FRotator SpawnRotation = Cast<APlayerController>(GetOwner())->GetPawn()->GetActorRotation();
			FActorSpawnParameters ActorSpawnParameters;

			Character_RTS = GetWorld()->SpawnActor<AETPCharacter>(Character_RTS_Class,SpawnLocation,SpawnRotation,ActorSpawnParameters);
			
			if (Cast<APlayerController>(GetOwner())&&Character_RTS)
			{
				Cast<APlayerController>(GetOwner())->Possess(Cast<APawn>(Character_RTS));
			}else
			{
				UToolLibrary::DebugLog("UStateComponent 的拥有者不是AplayerController");
			}
		}
		else
		{
			UToolLibrary::DebugLog("Character_RTS_Class or GetOwner() 为 NULL");
		}
	}
}
