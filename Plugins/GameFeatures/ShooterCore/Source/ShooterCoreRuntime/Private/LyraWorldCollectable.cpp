// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraWorldCollectable.h"

#include "Interaction/IInteractableTarget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraWorldCollectable)

struct FInteractionQuery;

ALyraWorldCollectable::ALyraWorldCollectable()
{
}

void ALyraWorldCollectable::GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder)
{
	InteractionBuilder.AddInteractionOption(Option);
}

FInventoryPickup ALyraWorldCollectable::GetPickupInventory() const
{
	return StaticInventory;
}
UMaterialInterface* ALyraWorldCollectable::GetInteractioningMaterial_Implementation()
{
	return Overlaymaterial;
}

void ALyraWorldCollectable::SetSelfInteractioning_Implementation(bool bInteractioning)
{
	if (bInteractioning)
	{
		TArray<UActorComponent*> Meshs;
		GetComponents(USkeletalMeshComponent::StaticClass(),Meshs);

		for (auto ActorComponent : Meshs)
		{
			if (USkeletalMeshComponent * Mesh = Cast<USkeletalMeshComponent>(ActorComponent))
			{
				Mesh->SetOverlayMaterial(GetInteractioningMaterial_Implementation());
			}
		}

		TArray<UActorComponent*> Meshs2;
		GetComponents(UStaticMeshComponent::StaticClass(),Meshs2);

		for (auto ActorComponent : Meshs2)
		{
			if (UStaticMeshComponent * Mesh = Cast<UStaticMeshComponent>(ActorComponent))
			{
				Mesh->SetOverlayMaterial(GetInteractioningMaterial_Implementation());
			}
		}
	}
	else
	{
		TArray<UActorComponent*> Meshs;
		GetComponents(USkeletalMeshComponent::StaticClass(),Meshs);

		for (auto ActorComponent : Meshs)
		{
			if (USkeletalMeshComponent * Mesh = Cast<USkeletalMeshComponent>(ActorComponent))
			{
				Mesh->SetOverlayMaterial(nullptr);
			}
		}

		TArray<UActorComponent*> Meshs2;
		GetComponents(UStaticMeshComponent::StaticClass(),Meshs2);

		for (auto ActorComponent : Meshs2)
		{
			if (UStaticMeshComponent * Mesh = Cast<UStaticMeshComponent>(ActorComponent))
			{
				Mesh->SetOverlayMaterial(nullptr);
			}
		}
	}
}