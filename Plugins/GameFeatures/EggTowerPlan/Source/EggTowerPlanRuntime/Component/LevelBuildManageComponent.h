// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "Inventory/LyraInventoryItemDefinition.h"
#include "Inventory/LyraInventoryManagerComponent.h"
#include "LevelBuildManageComponent.generated.h"

/**
 * 
 */
class ULyraExperienceDefinition;
UCLASS()
class EGGTOWERPLANRUNTIME_API ULevelBuildManageComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	ULevelBuildManageComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite)
	FLyraInventoryList AllBuildList;
	
	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite)
	FLyraInventoryList CanBuildList;

	UFUNCTION(BlueprintCallable)
	void InitBuildList();
	void OnExperienceLoaded(const ULyraExperienceDefinition*);
	UFUNCTION(BlueprintCallable,Category=LevelBuildManage)
	void AddBuild(TSubclassOf<ULyraInventoryItemDefinition> Item);
	UFUNCTION(BlueprintCallable,Category=LevelBuildManage)
	void AddMenu(TSubclassOf<ULyraInventoryItemDefinition> Item);
	UFUNCTION(BlueprintCallable,Category=LevelBuildManage)
	void RemoveBuild(TSubclassOf<ULyraInventoryItemDefinition> Item);

	UFUNCTION(BlueprintCallable, Category=LevelBuildManage, BlueprintPure=false)
	TArray<ULyraInventoryItemInstance*> GetAllCanBuild() const;

	UFUNCTION(BlueprintCallable, Category=LevelBuildManage, BlueprintPure=false)
	TArray<ULyraInventoryItemInstance*> GetAllAllBuild() const;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<TSubclassOf<ULyraInventoryItemDefinition>> BuildItemDefArray;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<TSubclassOf<ULyraInventoryItemDefinition>> AllBuildItemDefArray;
};