// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Items/BaseItem.h"
#include "ConsumableItem.generated.h"
DECLARE_LOG_CATEGORY_EXTERN(LogConsumableItem, Log, All);

class UGameplayEffect;
class ACHPlayable;
/**
 * 
 */
UCLASS()
class MOBAPROJECT_API UConsumableItem : public UBaseItem
{
	GENERATED_BODY()
protected:
	FActiveGameplayEffectHandle ConsumableHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> ConsumableEffect;

	UFUNCTION()
	bool ConsumeItem(const ACHPlayable* Character, float& OutDuration);
public:
	void Use(ACHPlayable* Character);

	/**
	 * Used to show an icon to say this effect is active
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnUse(ACHPlayable* Character, float EffectDuration);
};
