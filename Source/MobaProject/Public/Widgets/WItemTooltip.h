// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WItemTooltip.generated.h"

class UWItem;

/**
 * 
 */
UCLASS()
class MOBAPROJECT_API UWItemTooltip : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="Tooltip", meta=(ExposeOnSpawn = true))
	UWItem* ItemWidget;
};
