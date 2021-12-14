// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/AllMid/HUDAllMid.h"

#include "Blueprint/UserWidget.h"
#include "Widgets/AllMid/WGameScoreboard.h"
#include "Widgets/AllMid/WPlayerHud.h"
#include "Widgets/World/WItemsShop.h"

AHUDAllMid::AHUDAllMid()
{
}

void AHUDAllMid::BeginPlay()
{
	Super::BeginPlay();

	if (GameScoreboardClass)
	{
		GameScoreboard = CreateWidget<UWGameScoreboard>(GetOwningPlayerController(), GameScoreboardClass, FName(TEXT("GameScoreboard")));
		GameScoreboard->AddToViewport(0);
		GameScoreboard->SetVisibility(ESlateVisibility::Hidden);
	}
	if (PlayerHudClass)
	{
		PlayerHud = CreateWidget<UWPlayerHud>(GetOwningPlayerController(), PlayerHudClass, FName(TEXT("PlayerHud")));
		PlayerHud->AddToViewport(0);
	}
	if (ShopWidgetClass)
	{
		ShopWidget = CreateWidget<UWItemsShop>(GetOwningPlayerController(), ShopWidgetClass,TEXT("ShopWidget"));
		ShopWidget->SetVisibility(ESlateVisibility::Hidden);
		ShopWidget->AddToViewport(11);
		ShopWidget->InitShop(ItemsInShop);
	}
	bShopOpen = false;
}


void AHUDAllMid::ShowShop()
{
	ShopWidget->SetVisibility(ESlateVisibility::Visible);
	bShopOpen = true;
}

void AHUDAllMid::CloseShop()
{
	ShopWidget->SetVisibility(ESlateVisibility::Hidden);
	bShopOpen = false;
}

void AHUDAllMid::ShowScoreboard()
{
	GameScoreboard->SetVisibility(ESlateVisibility::Visible);
}

void AHUDAllMid::HideScoreboard()
{
	GameScoreboard->SetVisibility(ESlateVisibility::Hidden);
}
