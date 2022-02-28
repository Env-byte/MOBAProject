// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Framework/AllMid/PSAllMid.h"
#include "WGameScoreboardItem.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MOBAPROJECT_API UWGameScoreboardItem : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWidget(APSAllMid* PlayerState);
};
