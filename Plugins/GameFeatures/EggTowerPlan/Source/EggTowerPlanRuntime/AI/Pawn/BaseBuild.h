// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayCueInterface.h"
#include "ModularPawn.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "EggTowerPlanRuntime/Interaction/Interface/CombatInterface.h"
#include "EggTowerPlanRuntime/Tool/EnumLib.h"
#include "GameFramework/Actor.h"
#include "Interaction/IInteractableTarget.h"
#include "Inventory/LyraInventoryItemDefinition.h"
#include "Teams/LyraTeamAgentInterface.h"
#include "BaseBuild.generated.h"

class ULyraHealthSet;
class ULyraCombatSet;
class ULyraAbilitySystemComponent;
class ULyraHealthComponent;
class USphereComponent;
class UWidgetComponent;
UCLASS()
class EGGTOWERPLANRUNTIME_API ABaseBuild : public AModularPawn, public IInteractableTarget,public ICombatInterface,public IAbilitySystemInterface,public ILyraTeamAgentInterface, public IGameplayCueInterface
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

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETP|Build")
	USkeletalMeshComponent *Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETP|Build")
	UNiagaraComponent* NiagaraComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "ETP|Build")
	ULyraAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETP|Build", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULyraHealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "ETP|Defense")
	USceneComponent* RootSceneComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "ETP|Defense")
	USphereComponent* CollsionComp;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "ETP|Defense")
	UBoxComponent* BoxComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "ETP|Defense")
	UWidgetComponent *HPBar;

	//IInteractableTarget
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "ETP|Defense")
	UMaterialInterface* Overlaymaterial;
	
	UMaterialInterface* GetInteractioningMaterial_Implementation();

	virtual void SetSelfInteractioning_Implementation(bool bInteractioning) override;

	UFUNCTION(BlueprintCallable, Category= "Defense")
	void SetCurrentAiState(EDefenseState AIState);

	UFUNCTION(BlueprintImplementableEvent, Category= "Defense")
	void K2_OnSetCurrentAiState(EDefenseState AIState);
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "ETP|Defense")
	TSubclassOf<ULyraInventoryItemDefinition> ItemDefinition;
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentAIState)
	EDefenseState CurrentAIState;

public:
	//~ILyraTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnLyraTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of ILyraTeamAgentInterface interface
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Defense")
	FGenericTeamId TeamId = 0;
	UPROPERTY()
	FOnLyraTeamIndexChangedDelegate OnTeamChangedDelegate;

	/*CombatInterface*/
	void AttackStart_Implementation(FName Selection,FName AttackSocket = "");
	UFUNCTION(BlueprintNativeEvent)
	void Attack(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
	void AttackEnd_Implementation(FName Selection);
	
	FTimerHandle TimerHandle_Attacking;
	UFUNCTION(BlueprintNativeEvent)
	void UpdateAttack();
	// UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=IEnemyInterface)
	// bool bDrawDebugAttack = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=Attack)
	TSubclassOf<UGameplayEffect> GameplayEffect;
	
	UFUNCTION()
	void OnRep_CurrentAIState(EDefenseState AIState);

	UFUNCTION(BlueprintCallable, Category = "Defense")
	FORCEINLINE EDefenseState GetCurrentAiState() const { return CurrentAIState;}

	UFUNCTION(BlueprintCallable, Category = "Defense")
	FORCEINLINE AActor* GetTargetActor() const{ return  TargetActor;}

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Defense)
	class AActor* TargetActor;

	UFUNCTION()
	void OnTargetOnDestroyed(AActor* DestroyedActor );
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Defense)
	FVector BoxSizeVector = FVector(100.0F);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Defense)
	float DamageSphereRadius = 128.0f;
};
