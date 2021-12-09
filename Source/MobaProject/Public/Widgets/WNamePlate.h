// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WNamePlate.generated.h"

/**
 * 
 */
UCLASS()
class MOBAPROJECT_API UWNamePlate : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetHealthPercentage(const float Percent);

	UFUNCTION(BlueprintImplementableEvent)
	void SetManaPercentage(const float Percent);

	UFUNCTION(BlueprintImplementableEvent)
	void SetLevel(const int32 Level);

	UFUNCTION(BlueprintImplementableEvent)
	void SetName(const FName& Name);
};
