// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AllMid/WPlayerInventory.h"

#include "Items/BaseItem.h"
#include "Widgets/Components/WItem.h"

UWItem* UWPlayerInventory::FindFirstEmptySlot()
{
	for (UWItem* ItemSlot : ItemSlots)
	{
		UBaseItem* Item = ItemSlot->GetItem();
		if (!Item)
		{
			return ItemSlot;
		}
	}

	return nullptr;
}

UWItem* UWPlayerInventory::FindItemSlot(const FGuid UniqueId)
{
	for (UWItem* ItemSlot : ItemSlots)
	{
		UBaseItem* Item = ItemSlot->GetItem();
		if (!Item)
		{
			continue;
		}
		if (UniqueId.ToString() == Item->GetGuid().ToString())
		{
			return ItemSlot;
		}
	}
	return nullptr;
}

void UWPlayerInventory::OnInventoryUpdate(TArray<UBaseItem*> Items)
{
	//list of occupied slots
	TArray<UWItem*> SlotsOccupied;

	for (UBaseItem* Item : Items)
	{
		UWItem* ItemSlot;
		ItemSlot = FindItemSlot(Item->GetGuid());
		if (!ItemSlot)
		{
			//if item does not exist in inventory add it to first empty slot
			ItemSlot = FindFirstEmptySlot();
			ItemSlot->SetItem(Item);
		}

		ItemSlot->SetItem(Item);

		SlotsOccupied.Add(ItemSlot);
	}

	// find all slots that are not occupied and set their item to nullptr
	for (UWItem* ItemSlot : ItemSlots)
	{
		if (!SlotsOccupied.Contains(ItemSlot))
		{
			ItemSlot->SetItem(nullptr);
		}
	}
}
