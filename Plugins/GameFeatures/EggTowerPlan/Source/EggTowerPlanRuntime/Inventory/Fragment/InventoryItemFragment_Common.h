// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/LyraInventoryItemDefinition.h"
#include "InventoryItemFragment_Common.generated.h"

/**
 * 
 */
UCLASS()
class EGGTOWERPLANRUNTIME_API UInventoryItemFragment_Common : public ULyraInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Common)
	FText Name = FText::FromString("Name");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Common)
	TObjectPtr<USkeletalMesh> SkeletalMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Common)
	TObjectPtr<UStaticMesh> Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Common)
	FSlateBrush Icon;
};
