// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WGameOverview.generated.h"

/**
 * 
 */
UCLASS()
class MOBAPROJECT_API UWGameOverview : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTowersDestroyed(int32 RedTowers, int32 BlueTowers);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayersKilled(int32 Kills);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayerDeaths(int32 Deaths);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMinionsKilled(int32 MinionsKilled);
};
