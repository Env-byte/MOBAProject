// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/BaseItem.h"

#include "Characters/Playable/CHPlayable.h"
#include "Components/InventoryComponent.h"
#include "Framework/AllMid/PSAllMid.h"
#include "Framework/AllMid/PSAttributeSet.h"
#include "Net/UnrealNetwork.h"
DEFINE_LOG_CATEGORY(LogBaseItem);


UBaseItem::UBaseItem()
{
}

void UBaseItem::SetGuid()
{
	UniqueId = FGuid().NewGuid();
	MarkDirtyForReplication();
}

void UBaseItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UBaseItem, Quantity);
	DOREPLIFETIME(UBaseItem, UniqueId);
}

bool UBaseItem::IsSupportedForNetworking() const
{
	return true;
}

void UBaseItem::MarkDirtyForReplication()
{
	++RepKey;
	if (OwningInventory)
	{
		++OwningInventory->ReplicatedItemsKey;
	}
}

void UBaseItem::AddedToInventory(UInventoryComponent* Inventory)
{
}

void UBaseItem::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, bStackable ? MaxStackSize : 1);
		MarkDirtyForReplication();
	}
}

void UBaseItem::OnBuy(ACHPlayable* PlayerCharacter)
{
	UE_LOG(LogBaseItem, Display, TEXT("%s OnBuy"), *this->GetName())
}

void UBaseItem::OnSell(ACHPlayable* PlayerCharacter, APSAllMid* PlayerState)
{
	if (!PlayerState->HasAuthority()) return;
	UE_LOG(LogBaseItem, Display, TEXT("%s OnSell"), *this->GetName())
	const float Refund = GetSellPrice();
	const float CurrentGold = PlayerState->GetAttributeSet()->GetGold();
	PlayerState->GetAttributeSet()->SetGold(CurrentGold + Refund);
}

void UBaseItem::OnRep_Quantity()
{
	OnItemModified.Broadcast();
}

void UBaseItem::OnRep_UniqueId()
{
	OnItemModified.Broadcast();
}
