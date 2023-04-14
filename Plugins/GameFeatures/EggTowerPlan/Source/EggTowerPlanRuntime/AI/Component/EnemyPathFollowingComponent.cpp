// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPathFollowingComponent.h"

UEnemyPathFollowingComponent::UEnemyPathFollowingComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

FAIRequestID UEnemyPathFollowingComponent::RequestMove(const FAIMoveRequest& RequestData,
	FNavPathSharedPtr InPath)
{
	auto Id = Super::RequestMove(RequestData, InPath);
	
	bIsRequestMove = true;
	
	return Id;
}

inline FVector UEnemyPathFollowingComponent::GetDirection()
{
	return GetCurrentDirection();
}