// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraEmitter.h"
#include "Inventory/LyraInventoryItemDefinition.h"
#include "InventoryItemFragment_RTS.generated.h"

/**
 * 
 */
class UGameplayEffect;
UCLASS()
class EGGTOWERPLANRUNTIME_API UInventoryItemFragment_RTS : public ULyraInventoryItemFragment
{
	GENERATED_BODY()
public:
	//建造消耗
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=RTS)
	int32 Consume = 1;

	//攻击距离
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=RTS)
	float AttackLength = 1000.0f;

	//建筑物动画实例
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=RTS)
	TSubclassOf<UAnimInstance> AnimInstance;
	//攻击蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=RTS)
	UAnimMontage* AttackMontage;
	//攻击特效
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=RTS)
	UNiagaraSystem* NiagaraSystem;
	//攻击GE
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=RTS)
	TSubclassOf<UGameplayEffect> GameplayEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=RTS)
	float AttackRate = 1.0f;
};
