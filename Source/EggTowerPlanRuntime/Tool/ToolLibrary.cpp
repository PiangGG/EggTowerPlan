// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolLibrary.h"

void UToolLibrary::DebugLog(FString Message)
{
	if (bPrintDebugLog)
	{
		UE_LOG(LogTemp,Warning,TEXT("%s"),*Message);
	}
}
