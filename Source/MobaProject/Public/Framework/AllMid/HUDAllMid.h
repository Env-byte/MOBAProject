// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDAllMid.generated.h"

class UWPlayerHud;
class UWGameScoreboard;
/**
 * 
 */
UCLASS(Abstract)
class MOBAPROJECT_API AHUDAllMid : public AHUD
{
	GENERATED_BODY()
public:
	AHUDAllMid();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget Classes")
	TSubclassOf<UWGameScoreboard> GameScoreboardClass;

	UPROPERTY(BlueprintReadOnly, Category="Widgets")
	UWGameScoreboard* GameScoreboard;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget Classes")
	TSubclassOf<UWPlayerHud> PlayerHudClass;

	UPROPERTY(BlueprintReadOnly, Category="Widgets")
	UWPlayerHud* PlayerHud;
public:
	FORCEINLINE UWPlayerHud* GetPlayerHudWidget() const { return PlayerHud; };

	void ShowScoreboard();
	void HideScoreboard();
};
