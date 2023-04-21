// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EggTowerPlanRuntime/Tool/EnumLib.h"
#include "GameFramework/Actor.h"
#include "Interaction/IInteractableTarget.h"
#include "BaseBuild.generated.h"

class ULyraHealthSet;
class ULyraCombatSet;
class ULyraAbilitySystemComponent;
class ULyraHealthComponent;
class USphereComponent;
class UWidgetComponent;
UCLASS()
class EGGTOWERPLANRUNTIME_API ABaseBuild : public APawn, public IInteractableTarget
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseBuild(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;
	
	virtual void Destroyed() override;
	
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder) override;

	virtual void OnExperienceLoaded(const class ULyraExperienceDefinition* Definition);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=AbilitySet)
	ULyraHealthSet* HealthSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=AbilitySet)
	ULyraCombatSet* CombatSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent *Mesh;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "ETP|CoreUnit")
	ULyraAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETP|CoreUnit", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULyraHealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "ETP|CoreUnit")
	USphereComponent* CollsionComp;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "ETP|CoreUnit")
	UWidgetComponent *HPBar;

	//IInteractableTarget
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "ETP|CoreUnit")
	UMaterialInterface* Overlaymaterial;
	
	UMaterialInterface* GetInteractioningMaterial_Implementation();

	virtual void SetSelfInteractioning_Implementation(bool bInteractioning) override;

	UFUNCTION(BlueprintCallable, Category= "Defense")
	void SetCurrentAiState(EDefenseState AIState);

	UFUNCTION(BlueprintImplementableEvent, Category= "Defense")
	void K2_OnSetCurrentAiState(EDefenseState AIState);

private:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentAIState)
	EDefenseState CurrentAIState;

public:	
	UFUNCTION()
	void OnRep_CurrentAIState(EDefenseState AIState);

	UFUNCTION(BlueprintCallable, Category = "Defense")
	FORCEINLINE EDefenseState GetCurrentAiState() const { return CurrentAIState;}

	UFUNCTION(BlueprintCallable, Category = "Defense")
	FORCEINLINE AActor* GetTargetActor() const{ return  TargetActor;}

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Defense)
	class AActor* TargetActor;
};
