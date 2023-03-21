// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange,AActor*,FromActor,float,LossHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EGGTOWERPLANRUNTIME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void OnOwnerTakeAnyDamge(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(BlueprintAssignable,Category=Events)
	FOnHealthChange OnHealthChange;

	bool ApplyChangeHealth();
private:
	float MaxHealth = 100.0f;
	
	float CurrentHealth = 100.0;
};

