// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "BaseEnemy.generated.h"
class UHealthComponent;
UCLASS()
class EGGTOWERPLANRUNTIME_API ABaseEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//
	virtual void PostInitializeComponents() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Component")
	USceneComponent* MeshRootComponent;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Component")
	UStaticMeshComponent* ST_MeshComp;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Component")
	USkeletalMeshComponent* SK_MeshComp;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Component")
	UWidgetComponent* WidgetComp_Cooling;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Component")
	UArrowComponent* ArrowComponent;
};
