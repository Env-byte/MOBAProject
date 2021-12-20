// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/WItem.h"

#include "Items/BaseItem.h"

void UWItem::NativeConstruct()
{
	Item = nullptr;
	Super::NativeConstruct();
}

void UWItem::SetItem(UBaseItem* InItem)
{
	Item = InItem;
	if (Item)
	{
		Item->ItemSlotRef = this;
	}
	BP_OnItemChange();
}
