// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreUnitManageComponent.h"
#include "NativeGameplayTags.h"
#include "EggTowerPlanRuntime/Tool/StructLib.h"
#include "GameFramework/GameplayMessageSubsystem.h"

//UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_CoreUnit_Change,"ETP.CoreUnit.ChangeCoreUnitNumber");
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_CoreUnit_Change);

UCoreUnitManageComponent::UCoreUnitManageComponent(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
	
}

void UCoreUnitManageComponent::BeginPlay()
{
	Super::BeginPlay();

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	ListenerHandle = MessageSubsystem.RegisterListener(TAG_CoreUnit_Change, this, &ThisClass::OnChangeCoreUnitMessage);
}

void UCoreUnitManageComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(ListenerHandle);
}

void UCoreUnitManageComponent::OnChangeCoreUnitMessage(FGameplayTag Channel, const FChangeCoreUnitMessage& Message)
{
	switch (Message.CoreUnitChange)
	{
		case ECoreUnitChange::ECoreUnitAdd:
			CoreUnit.AddUnique(Message.CoreUnit);
			break;
		case ECoreUnitChange::ECoreUnitRemove:
			if(CoreUnit.IsValidIndex(CoreUnit.Find(Message.CoreUnit)))
			{
				CoreUnit.RemoveAt(CoreUnit.Find(Message.CoreUnit));
			}
			break;
		case ECoreUnitChange::ENULL:
			break;
		default: ;
	}
}
