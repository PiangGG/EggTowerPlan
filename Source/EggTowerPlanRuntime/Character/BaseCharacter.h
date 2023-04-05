// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularCharacter.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "GameplayCueInterface.h"
#include "AbilitySystem/Attributes/LyraCombatSet.h"
#include "AbilitySystem/Attributes/LyraHealthSet.h"
#include "GameModes/LyraExperienceDefinition.h"
#include "Teams/LyraTeamAgentInterface.h"
#include "BaseCharacter.generated.h"

/**
 * 
 */
class ULyraHealthSet;
class ULyraCombatSet;
class ULyraExperienceDefinition;
class ULyraAbilitySystemComponent;
class ULyraHealthComponent;
class ULyraCameraComponent;
class UETPHeroComponent;

UCLASS(Blueprintable, BlueprintType)
class EGGTOWERPLANRUNTIME_API ABaseCharacter : public AModularCharacter, public IAbilitySystemInterface, public ILyraTeamAgentInterface, public IGameplayCueInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void PostInitializeComponents() override;

	virtual ULyraAbilitySystemComponent* GetLyraAbilitySystemComponent();
	void OnExperienceLoaded(const ULyraExperienceDefinition*);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//~ILyraTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnLyraTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of ILyraTeamAgentInterface interface

	// Begins the death sequence for the character (disables collision, disables movement, etc...)
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);

	// Ends the death sequence for the character (detaches controller, destroys pawn, etc...)
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

	// Called when the death sequence for the character has completed
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnDeathFinished"))
	void K2_OnDeathFinished();
	
	void DisableMovementAndCollision();
	void DestroyDueToDeath();
	void UninitAndDestroy();

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	TSubclassOf<ULyraCameraMode> DetermineCameraMode() const;
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
		
public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Team")
	FGenericTeamId TeamId = 0;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ULyraHealthSet* HealthSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ULyraCombatSet* CombatSet;


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETP|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULyraHealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETP|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULyraCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETP|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UETPHeroComponent> HeroComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "ETP|Charactor")
	ULyraAbilitySystemComponent* AbilitySystemComponent;
};
