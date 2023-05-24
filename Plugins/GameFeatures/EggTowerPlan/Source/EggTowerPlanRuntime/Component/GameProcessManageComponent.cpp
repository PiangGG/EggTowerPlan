// Fill out your copyright notice in the Description page of Project Settings.


#include "GameProcessManageComponent.h"

#include "GameModes/LyraExperienceManagerComponent.h"

UGameProcessManageComponent::UGameProcessManageComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void UGameProcessManageComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() != NM_Client)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);
		ULyraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}
}

void UGameProcessManageComponent::OnExperienceLoaded(const ULyraExperienceDefinition*)
{
	IntoMapStartGame();
}

void UGameProcessManageComponent::EndGame_Implementation()
{
	
}

void UGameProcessManageComponent::IntoMapStartGame_Implementation()
{
	UE_LOG(LogTemp,Warning,TEXT("Start"))
}
