// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "MapUnit.generated.h"

UCLASS()
class EGGTOWERPLANRUNTIME_API AMapUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapUnit();

	virtual FVector GetBuildLocation();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;
	//Component
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Comonent|SceneComponent")
	USceneComponent* RootComp;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Comonent|CollsionComponent")
	UBoxComponent* CollsionComp;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Comonent|MeshComponent")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Base|ShowMesh")
	bool bShowStaticMesh = false;
	UFUNCTION(BlueprintCallable)
	void SetUnitVisble(bool bShow);
	UFUNCTION(BlueprintCallable)
	bool GetUnitVisble();
	UFUNCTION(BlueprintCallable)
	void UpdateUnitMateria(float DeltaTime);
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Base|ShowMesh")
	FVector BoxExtent = FVector(50.0);
	
	UFUNCTION(BlueprintCallable)
	bool GetbOverlapActor(AActor* ignoreActor);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base|Material")
	UMaterialInterface *Material_G;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base|Material")
	UMaterialInterface *Material_R;

	UPROPERTY()
	TArray<AActor*> ignoreActors;
	UFUNCTION(BlueprintCallable)
	void AddIgnoreTraceActor(AActor* Actor);
	UFUNCTION(BlueprintCallable)
	void RemoveIgnoreTraceActor(AActor* Actor);

	UPROPERTY()
	bool HaveActor = false;
	UFUNCTION()
	bool GetbHaveActor();

	//在地圖單元上面的Actor
	UPROPERTY()
	AActor* OnActor = nullptr;
	UFUNCTION(BlueprintCallable)
	void SetOnActor(AActor* actor);
};


