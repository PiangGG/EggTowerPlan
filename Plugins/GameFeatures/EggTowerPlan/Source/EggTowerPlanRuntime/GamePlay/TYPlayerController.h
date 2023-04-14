// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "EggTowerPlanRuntime/Tool/EnumLib.h"

#include "TYPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EGGTOWERPLANRUNTIME_API ATYPlayerController : public AModularPlayerController
{
	GENERATED_BODY()
	
	ATYPlayerController();
public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Attr|射线基础长度")
	float LineLength = 10000.0f;
	
	UPROPERTY()
	AActor* SelectedActor;
private:
	
	AActor* HoverActor;

	//当前鼠标所在地图单位
	UPROPERTY()
	AActor* CurrentHoverMapUnit;
	//上一次鼠标所在地图单位
	UPROPERTY()
	AActor* LastHoverMapUnit;
public:
	//控制器状态
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="")
	EControllerState ControllerState = EControllerState::ECombat;
	
	UFUNCTION(BlueprintCallable)
	void SetControllerState(EControllerState State);

	UFUNCTION(BlueprintCallable)
	EControllerState GetControllerState();

	void UpdateSelectedActor(float DeltaSeconds);

	/*处理鼠标点击Actor*/
	virtual void OnClick(AActor* TargetActor);

	//鼠标屏幕位置向控制器方向是否命中地面
	UFUNCTION(BlueprintCallable)
	bool GetOnClickbMapUnitActor();
	
	/*设置选中Actor*/
	virtual void SetSelectedActor(AActor* TargetActor);

	/*
	 * 战斗状态下点击
	 */
	virtual void SelectedActorAttack(AActor* TargetActor);

	virtual void SetActorLocationToUnitMap(AActor* TargetActor);

	virtual void LineTraceMapUnit();

	virtual void OnClickStarted();
	
	virtual void OnClickCompleted();

	bool bOnCilcking = false;
private:
	
};