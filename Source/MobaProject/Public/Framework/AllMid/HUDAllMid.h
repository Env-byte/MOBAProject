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
	////////// Scoreboard ///////////
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget Classes")
	TSubclassOf<UWGameScoreboard> GameScoreboardClass;


public:
	UPROPERTY(BlueprintReadOnly, Category="Scoreboard Widgets")
	UWGameScoreboard* GameScoreboard;
	void ShowScoreboard();
	void HideScoreboard();
	////////// Scoreboard ///////////

protected:
	////////// PlayerHud ///////////
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget Classes")
	TSubclassOf<UWPlayerHud> PlayerHudClass;

	UPROPERTY(BlueprintReadOnly, Category="PlayerHud Widget")
	UWPlayerHud* PlayerHud;
public:
	FORCEINLINE UWPlayerHud* GetPlayerHudWidget() const { return PlayerHud; };
	////////// PlayerHud ///////////
};
