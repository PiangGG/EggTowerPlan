// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayCueInterface.h"
#include "GameplayEffect.h"
#include "ModularCharacter.h"
#include "Components/BoxComponent.h"
#include "EggTowerPlanRuntime/AI/Interface/EnemyInterface.h"
#include "EggTowerPlanRuntime/Interaction/Interface/CombatInterface.h"
#include "EggTowerPlanRuntime/Tool/EnumLib.h"
#include "Teams/LyraTeamAgentInterface.h"
#include "BaseEnemy.generated.h"

/**
 * 
 */
class ULyraAbilitySystemComponent;
class ULyraExperienceDefinition;
class ULyraHealthSet;
class ULyraCombatSet;
UCLASS(Blueprintable, BlueprintType)
class EGGTOWERPLANRUNTIME_API ABaseEnemy : public AModularCharacter, public IEnemyInterface ,public ICombatInterface,public IAbilitySystemInterface,public ILyraTeamAgentInterface, public IGameplayCueInterface
{
	GENERATED_BODY()
public:
	ABaseEnemy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(VisibleAnywhere, Category = "ETP|Enemy")
	ULyraAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "ETP|Enemy")
	UBoxComponent* AttackBoxCollsion;
	
	virtual void PostInitializeComponents() override;
	void OnExperienceLoaded(const ULyraExperienceDefinition*);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 覆盖方法，让 AI 的视线 根据socket 来动
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	//~ILyraTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnLyraTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of ILyraTeamAgentInterface interface

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	UFUNCTION(BlueprintCallable, Category= "Enemy")
	void SetCurrentAiState(EEnemyState AIState);

	UFUNCTION(BlueprintImplementableEvent, Category= "Enemy")
	void K2_OnSetCurrentAiState(EEnemyState AIState);
	
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	FORCEINLINE EEnemyState GetCurrentAiState() const { return CurrentAIState;}

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	FORCEINLINE AActor* GetTargetActor() const{ return  TargetActor;}
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	FORCEINLINE FVector GetTargetMoveLocation() const{ return  TargeLocation;}
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	FORCEINLINE FVector GetTargetLastMoveLocation() const{ return  TargetLastLocation;}

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy")
	FGenericTeamId TeamId = 1;
protected:
	
	UPROPERTY()
	FOnLyraTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ULyraHealthSet* HealthSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ULyraCombatSet* CombatSet;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Enemy)
	class AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=Enemy)
	FVector TargeLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=Enemy)
	FVector TargetLastLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=IEnemyInterface)
	UAnimMontage*AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=IEnemyInterface)
	float MaxAttackLength = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=IEnemyInterface)
	FName DefaultAttackSocket = "DefaultAttackSocket";
private:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentAIState)
	EEnemyState CurrentAIState;

	UFUNCTION()
	void OnRep_CurrentAIState(EEnemyState AIState);
public:
	//UFUNCTION(BlueprintCallable)
	UAnimMontage* GetAnimMontage_Implementation() override;

	//UFUNCTION(BlueprintCallable)
	float GetMaxAttackLength_Implementation() override;

	/*CombatInterface*/
	void AttackStart_Implementation(FName Selection,FName AttackSocket = "");
	UFUNCTION(BlueprintNativeEvent)
	void Attack(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
	void AttackEnd_Implementation(FName Selection);

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=Attack)
	TSubclassOf<UGameplayEffect> GameplayEffect;
	
	virtual void BeHit_Implementation(FName Selection) override;
};


