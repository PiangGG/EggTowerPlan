// Fill out your copyright notice in the Description page of Project Settings.


#include "TYMapGameInstanceSubsystem.h"
#include "MapUnit.h"
#include "Kismet/GameplayStatics.h"

UTYMapGameInstanceSubsystem::UTYMapGameInstanceSubsystem()
{
	
}

void UTYMapGameInstanceSubsystem::InitMap()
{
	for (int i = 0;i<32;i++)
	{
		for (int j = 0;j<32;j++)
		{
			FVector Location;
			Location.X = j*100;
			Location.Y = i*100; 
			GetWorld()->SpawnActor<AMapUnit>(Location,FRotator(0));
		}
	}
}

void UTYMapGameInstanceSubsystem::SetUnitVisble(bool bVisble)
{
	/*if (bVisble)
	{
		
	}
	else
	{
		
	}*/
	TArray<AActor*>Units;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AMapUnit::StaticClass(),Units);
	for (auto& Unit : Units)
	{
		Cast<AMapUnit>(Unit)->SetUnitVisble(bVisble);
	}
}
