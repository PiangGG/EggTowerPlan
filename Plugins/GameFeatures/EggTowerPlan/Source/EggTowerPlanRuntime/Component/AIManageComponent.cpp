// Fill out your copyright notice in the Description page of Project Settings.


#include "AIManageComponent.h"

UAIManageComponent::UAIManageComponent(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAIManageComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAIManageComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UAIManageComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAIManageComponent::RegisterBuild(ABaseBuild* Build)
{
	if (!BuildList.Contains(Build))
	{
		BuildList.Add(Build);
	}
}

void UAIManageComponent::UnRegisterBuild(ABaseBuild* Build)
{
	if (BuildList.Contains(Build))
	{
		BuildList.Remove(Build);
	}
}

void UAIManageComponent::RegisterEnemy(ABaseEnemy* Enemy)
{
	if (!EnemyList.Contains(Enemy))
	{
		EnemyList.Add(Enemy);
	}
}

void UAIManageComponent::UnRegisterEnemy(ABaseEnemy* Enemy)
{
	if (EnemyList.Contains(Enemy))
	{
		EnemyList.Remove(Enemy);
	}
}

int32 UAIManageComponent::GetEnemyNum()
{
	return EnemyList.Num();
}

int32 UAIManageComponent::GetBuildNum()
{
	return  BuildList.Num();
}
