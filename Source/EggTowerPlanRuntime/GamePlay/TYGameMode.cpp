// Fill out your copyright notice in the Description page of Project Settings.


#include "TYGameMode.h"

#include "ControPawn.h"
#include "TYPlayerController.h"

ATYGameMode::ATYGameMode()
{
	DefaultPawnClass = AControPawn::StaticClass();
	PlayerControllerClass = ATYPlayerController::StaticClass();
}
