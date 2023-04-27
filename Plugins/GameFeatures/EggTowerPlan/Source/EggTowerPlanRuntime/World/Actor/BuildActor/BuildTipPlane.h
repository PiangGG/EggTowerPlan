// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildTipPlane.generated.h"

struct FBuildUnit;
UCLASS()
class EGGTOWERPLANRUNTIME_API ABuildTipPlane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildTipPlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void InitUnit();
	UFUNCTION(BlueprintCallable)
	virtual void ShowUnit(int32 Size = 2);
	UFUNCTION(BlueprintCallable)
	virtual void HideUnit();
public:
	virtual void UnitBeginOverlap(UPrimitiveComponent* OtherComp);
	virtual void UnitEndOverlap(UPrimitiveComponent* OtherComp);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMesh*StaticMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USceneComponent* RootSceneComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<UStaticMeshComponent*>Meshs;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FBuildUnit>MeshsOverlapMap;
	
	// UPROPERTY(EditAnywhere,BlueprintReadWrite)
	// UInstancedStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 length = 4;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Maxlength = 5;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base|Material")
	UMaterialInterface *Material_G;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base|Material")
	UMaterialInterface *Material_R;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 OverlapNumber = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bCanBuild = true;
};
