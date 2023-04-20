// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelBuildManageComponent.h"

#include "GameModes/LyraExperienceManagerComponent.h"
#include "GameFramework/GameState.h"
#include "Inventory/LyraInventoryItemInstance.h"
#include "Net/UnrealNetwork.h"
#include "UObject/Object.h"
ULevelBuildManageComponent::ULevelBuildManageComponent(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
, CanBuildList(this)
{
	
}

void ULevelBuildManageComponent::BeginPlay()
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

void ULevelBuildManageComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CanBuildList);
}

void ULevelBuildManageComponent::InitBuildList()
{
	for (TSubclassOf<ULyraInventoryItemDefinition>  BuildItem : BuildItemDefArray)
	{
		AddBuild(BuildItem);
	}
}

void ULevelBuildManageComponent::OnExperienceLoaded(const ULyraExperienceDefinition*)
{
	InitBuildList();
}

void ULevelBuildManageComponent::AddBuild(TSubclassOf<ULyraInventoryItemDefinition> ItemDef)
{
	ULyraInventoryItemInstance* Result = nullptr;
	if (ItemDef != nullptr)
	{
		Result = CanBuildList.AddEntry(ItemDef, 1);
		
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}
}

void ULevelBuildManageComponent::RemoveBuild(TSubclassOf<ULyraInventoryItemDefinition> ItemDef)
{
	for (const FLyraInventoryEntry& Entry : CanBuildList.Entries)
	{
		ULyraInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				CanBuildList.RemoveEntry(Instance);

				if (Instance && IsUsingRegisteredSubObjectList())
				{
					RemoveReplicatedSubObject(Instance);
				}
			}
		}
	}
}

TArray<ULyraInventoryItemInstance*> ULevelBuildManageComponent::GetAllCanBuild() const
{
	return CanBuildList.GetAllItems();
}
