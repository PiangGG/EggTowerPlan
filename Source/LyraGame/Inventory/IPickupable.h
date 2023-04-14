// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Containers/Array.h"
#include "HAL/Platform.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Templates/SubclassOf.h"
#include "UObject/Interface.h"
#include "UObject/ScriptInterface.h"
#include "UObject/UObjectGlobals.h"

#include "IPickupable.generated.h"

class AActor;
class ULyraInventoryItemDefinition;
class ULyraInventoryItemInstance;
class ULyraInventoryManagerComponent;
class UObject;
struct FFrame;

USTRUCT(BlueprintType)
struct LYRAGAME_API FPickupTemplate
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int32 StackCount = 1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<ULyraInventoryItemDefinition> ItemDef;
};

USTRUCT(BlueprintType)
struct LYRAGAME_API FPickupInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<ULyraInventoryItemInstance> Item = nullptr;
};

USTRUCT(BlueprintType)
struct LYRAGAME_API FInventoryPickup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FPickupInstance> Instances;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FPickupTemplate> Templates;
};

/**  */
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UPickupable : public UInterface
{
	GENERATED_BODY()
};

/**  */
class IPickupable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual FInventoryPickup GetPickupInventory() const = 0;
};

/**  */
UCLASS()
class UPickupableStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UPickupableStatics();

public:
	UFUNCTION(BlueprintPure)
	static TScriptInterface<IPickupable> GetFirstPickupableFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, meta = (WorldContext = "Ability"))
	static void AddPickupToInventory(ULyraInventoryManagerComponent* InventoryComponent, TScriptInterface<IPickupable> Pickup);
};
