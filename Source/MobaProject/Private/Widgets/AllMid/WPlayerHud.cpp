// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AllMid/WPlayerHud.h"
#include "Widgets/AllMid/WItemsShop.h"
#include "Widgets/AllMid/WPlayerInventory.h"

void UWPlayerHud::ShowShop()
{
	ShopWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	bIsShopOpen = true;
}

void UWPlayerHud::HideShop()
{
	ShopWidget->SetVisibility(ESlateVisibility::Collapsed);
	bIsShopOpen = false;
}

void UWPlayerHud::SetInventory(const TArray<UBaseItem*>& Items)
{
	PlayerInventory->OnInventoryUpdate(Items);
}
