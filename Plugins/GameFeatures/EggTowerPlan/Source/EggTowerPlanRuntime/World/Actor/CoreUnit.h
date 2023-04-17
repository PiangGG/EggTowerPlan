// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Interaction/IInteractableTarget.h"
#include "CoreUnit.generated.h"
class ULyraHealthSet;
class ULyraCombatSet;
class ULyraAbilitySystemComponent;
class ULyraHealthComponent;
UCLASS()
class EGGTOWERPLANRUNTIME_API ACoreUnit : public AActor, public IInteractableTarget
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoreUnit(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;
	
	virtual void Destroyed() override;
	
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder) override;

	void OnExperienceLoaded(const class ULyraExperienceDefinition*);

	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

	// Called when the death sequence for the character has completed
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnDeathFinished"))
	void K2_OnDeathFinished();
	
	void DisableMovementAndCollision();
	void DestroyDueToDeath();
	void UninitAndDestroy();
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);
	
	UPROPERTY(EditAnywhere)
	FInteractionOption Option;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ULyraHealthSet* HealthSet;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ULyraCombatSet* CombatSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent *Mesh;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "ETP|CoreUnit")
	ULyraAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETP|CoreUnit", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULyraHealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "ETP|CoreUnit")
	USphereComponent* CollsionComp;
};
