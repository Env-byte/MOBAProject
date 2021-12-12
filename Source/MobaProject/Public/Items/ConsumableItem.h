// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "ConsumableItem.generated.h"

class ACHPlayable;
/**
 * 
 */
UCLASS()
class MOBAPROJECT_API UConsumableItem : public UBaseItem
{
	GENERATED_BODY()

public:
	void Use(ACHPlayable* Character);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnUse(ACHPlayable* Character);
};
