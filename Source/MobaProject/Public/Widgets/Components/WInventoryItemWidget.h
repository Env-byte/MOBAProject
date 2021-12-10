// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WInventoryItemWidget.generated.h"
class UBaseItem;
/**
 * 
 */
UCLASS()
class MOBAPROJECT_API UWInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="Inventory Item Widget", meta=(ExposeOnSpawn=true))
	UBaseItem* Item;
};
