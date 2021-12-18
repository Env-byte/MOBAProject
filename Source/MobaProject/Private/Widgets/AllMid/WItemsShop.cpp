// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AllMid/WItemsShop.h"

void UWItemsShop::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemsInShop.Num() > 0)
	{
		InitShop(ItemsInShop);
	}
}
