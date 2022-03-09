// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WGameScoreboardItem.generated.h"

class APSAllMid;
/**
 * 
 */
UCLASS(Abstract)
class MOBAPROJECT_API UWGameScoreboardItem : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly)
	APSAllMid* PlayerState;

public:
	void SetPlayerState(APSAllMid* ThisPlayerState);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWidget();
};
