// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/BaseItem.h"

#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"

UBaseItem::UBaseItem()
{
}

void UBaseItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UBaseItem, Quantity)
}

bool UBaseItem::IsSupportedForNetworking() const
{
	return UObject::IsSupportedForNetworking();
}

void UBaseItem::MarkDirtyForReplication()
{
	++RepKey;
	if (OwningInventory)
	{
		++OwningInventory->ReplicatedItemsKey;
	}
}

void UBaseItem::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, bStackable ? MaxStackSize : 1);
		MarkDirtyForReplication();
	}
}

void UBaseItem::Use(ACHBase* Character)
{
}

void UBaseItem::AddedToInventory(UInventoryComponent* Inventory)
{
}

void UBaseItem::OnRep_Quantity()
{
	OnItemModified.Broadcast();
}
