// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WAbility.generated.h"

/**
 * 
 */
UCLASS()
class MOBAPROJECT_API UWAbility : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void StartCountdown(const float RemainingTime, const float CooldownTime);
};
