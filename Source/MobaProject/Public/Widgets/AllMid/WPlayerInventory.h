// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WPlayerInventory.generated.h"

class UBaseItem;
class UWItem;

/**
 * 
 */
UCLASS()
class MOBAPROJECT_API UWPlayerInventory : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, Category=Items)
	TArray<UWItem*> ItemSlots;

	UWItem* FindFirstEmptySlot();
	UWItem* FindItemSlot(FGuid UniqueId);
public:
	void OnInventoryUpdate(TArray<UBaseItem*> Items);
};
