// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/BaseItem.h"
#include "WPlayerHud.generated.h"

class UCHAttributeSet;
/**
 * 
 */
UCLASS(Abstract)
class MOBAPROJECT_API UWPlayerHud : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void BP_SetCharacterStats(UCHAttributeSet* AttributeSet);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_SetHealth(const float Health, const float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_SetMana(const float Mana, const float MaxMana);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_SetLevel(const int32 Level);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_SetGold(const float InGold);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_SetInventory(const TArray<UBaseItem*>& Items);
};
