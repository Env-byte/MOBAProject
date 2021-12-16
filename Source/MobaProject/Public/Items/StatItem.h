// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "GameplayEffectTypes.h"
#include "StatItem.generated.h"
DECLARE_LOG_CATEGORY_EXTERN(LogStatItem, Log, All);

class UGameplayEffect;
class ACHPlayable;
/**
 * 
 */
UCLASS()
class MOBAPROJECT_API UStatItem : public UBaseItem
{
	GENERATED_BODY()

protected:
	FActiveGameplayEffectHandle StatHandle;

	/**
	 * Apply the stats to this character, keep a reference to the handle to remove the stats when item is sold
	 */
	void ApplyStats(const ACHPlayable* PlayerCharacter);
	/**
	 * Remove the stats from this character
	 */
	void RemoveStats(const ACHPlayable* PlayerCharacter) const;
public:
	virtual void OnBuy(ACHPlayable* PlayerCharacter) override;
	virtual void OnSell(ACHPlayable* PlayerCharacter,APSAllMid* PlayerState) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Stats)
	TSubclassOf<UGameplayEffect> ItemStats;
};
