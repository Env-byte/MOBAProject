// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widgets/World/WItemsShop.h"
#include "HUDAllMid.generated.h"

class UBaseItem;
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

	UPROPERTY(BlueprintReadOnly, Category="Scoreboard Widgets")
	UWGameScoreboard* GameScoreboard;
public:
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

	////////// Shop ///////////
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget Classes")
	TSubclassOf<UWItemsShop> ShopWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category="Shop Widget")
	UWItemsShop* ShopWidget;

	UPROPERTY(EditDefaultsOnly, Category="Shop")
	TArray<TSubclassOf<UBaseItem>> ItemsInShop;
public:
	bool bShopOpen = false;

	UFUNCTION(BlueprintCallable)
	void ShowShop();

	UFUNCTION(BlueprintCallable)
	void CloseShop();

	FORCEINLINE UWItemsShop* GetShopWidget() const { return ShopWidget; };
	////////// Shop //////////
};
