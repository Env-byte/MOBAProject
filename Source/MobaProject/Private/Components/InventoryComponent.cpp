// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Items/BaseItem.h"
#define  LOCTEXT_NAMESPACE "Inventory"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	SetIsReplicatedByDefault(true);
}

FItemAddResult UInventoryComponent::TryAddItem(UBaseItem* Item)
{
	return TryAddItem_Internal(Item);
}

FItemAddResult UInventoryComponent::TryAddItemFromClass(TSubclassOf<UBaseItem> ItemClass, const int32 Quantity)
{
	UBaseItem* Item = NewObject<UBaseItem>(GetOwner(), ItemClass);
	Item->SetQuantity(Quantity);
	return TryAddItem_Internal(Item);
}

int32 UInventoryComponent::ConsumeItem(UBaseItem* Item)
{
	if (Item)
	{
		ConsumeItem(Item, Item->GetQuantity());
	}
	return 0;
}

int32 UInventoryComponent::ConsumeItem(UBaseItem* Item, const int32 Quantity)
{
	if (GetOwner() && GetOwner()->HasAuthority() && Item)
	{
		const int32 RemoveQuantity = FMath::Min(Quantity, Item->GetQuantity());
		const int32 NewQuantity = Item->GetQuantity() - RemoveQuantity;
		//we should not hit a negative value
		ensure(!(NewQuantity < 0));

		//we now have zero of ths item remove it from the inventory
		Item->SetQuantity(NewQuantity);
		if (Item->GetQuantity() == 0)
		{
			RemoveItem(Item);
		}
		else
		{
			ClientRefreshInventory();
		}
		return RemoveQuantity;
	}
	return 0;
}

bool UInventoryComponent::RemoveItem(UBaseItem* Item)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		if (Item)
		{
			Items.RemoveSingle(Item);

			ReplicatedItemsKey++;
			return true;
		}
	}

	return false;
}

bool UInventoryComponent::HasItem(const TSubclassOf<UBaseItem> ItemClass, const int32 Quantity) const
{
	if (const UBaseItem* ItemToFind = FindItemByClass(ItemClass))
	{
		return ItemToFind->GetQuantity() >= Quantity;
	}
	return false;
}

UBaseItem* UInventoryComponent::FindItem(UBaseItem* Item) const
{
	if (Item)
	{
		for (const auto InvItem : Items)
		{
			if (InvItem && InvItem->GetClass() == Item->GetClass())
			{
				return InvItem;
			}
		}
	}
	return nullptr;
}

UBaseItem* UInventoryComponent::FindItemByClass(const TSubclassOf<UBaseItem> ItemClass) const
{
	for (auto& InvItem : Items)
	{
		if (InvItem && InvItem->GetClass() == ItemClass)
		{
			return InvItem;
		}
	}
	return nullptr;
}

TArray<UBaseItem*> UInventoryComponent::FindItemsByClass(const TSubclassOf<UBaseItem> ItemClass) const
{
	TArray<UBaseItem*> ItemsOfClass;
	for (auto& InvItem : Items)
	{
		if (InvItem && InvItem->GetClass()->IsChildOf(ItemClass))
		{
			ItemsOfClass.Add(InvItem);
		}
	}
	return ItemsOfClass;
}

void UInventoryComponent::SetCapacity(const int32 NewCapacity)
{
	Capacity = NewCapacity;
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::ClientRefreshInventory_Implementation()
{
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryComponent, Items);
}

bool UInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (Channel->KeyNeedsToReplicate(0, ReplicatedItemsKey))
	{
		for (const auto& Item : Items)
		{
			if (Channel->KeyNeedsToReplicate(Item->GetUniqueID(), Item->RepKey))
			{
				bWroteSomething |= Channel->ReplicateSubobject(Item, *Bunch, *RepFlags);
			}
		}
	}

	return bWroteSomething;
}

void UInventoryComponent::OnRep_Items()
{
	OnInventoryUpdated.Broadcast();
}

FItemAddResult UInventoryComponent::TryAddItem_Internal(UBaseItem* Item)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		const int32 AddAmount = Item->GetQuantity();
		if (Items.Num() + 1 > GetCapacity())
		{
			const FText Error = LOCTEXT("InventoryCapacityFullText", "Could not addd item to Inventory. Inventory is full.");
			return FItemAddResult::AddedNone(AddAmount, Error);
		}

		//if the item is stackable, check if we already have it and add it to their stack
		if (Item->bStackable)
		{
			//somehow the items quantity went over the max stack size. this should never happen
			ensure(Item->GetQuantity() <= Item->MaxStackSize);

			if (UBaseItem* ExistingItem = FindItem(Item))
			{
				if (ExistingItem->GetQuantity() < ExistingItem->MaxStackSize)
				{
					const int32 CapacityMaxAddAmount = ExistingItem->MaxStackSize - ExistingItem->GetQuantity();
					int32 ActualAddAmount = FMath::Min(AddAmount, CapacityMaxAddAmount);
					FText ErrorText = LOCTEXT("InventoryErrorText", "Could not add all of the items to your Inventory.");

				
					 if (ActualAddAmount < AddAmount)
					{
						//if the item weighs none and we cant take it, then there was a capacity issue (not enough inventory slots)
						ErrorText = FText::Format(LOCTEXT("InventoryCapacityFullText",
						                                  "Could not add entire stack of {ItemName} to Inventory. Inventory was full"), Item->DisplayName);
					}

					if (ActualAddAmount <= 0)
					{
						return FItemAddResult::AddedNone(AddAmount,LOCTEXT("InventoryErrorText", "Could not add item to Inventory."));
					}

					ExistingItem->SetQuantity(ExistingItem->GetQuantity() + ActualAddAmount);

					//if we somehow get more of the item than the max stack size then something is wrong with our math
					ensure(ExistingItem->GetQuantity() <= ExistingItem->MaxStackSize);

					if (ActualAddAmount < AddAmount)
					{
						return FItemAddResult::AddedSome(AddAmount, ActualAddAmount, ErrorText);
					}
					else
					{
						return FItemAddResult::AddedAll(AddAmount);
					}
				}
				return FItemAddResult::AddedNone(AddAmount, FText::Format(LOCTEXT("InventoryFullStackText", "Could not add {ItemName}. You already have a full stack of this item"), Item->DisplayName));
			}
			AddItem(Item);
			return FItemAddResult::AddedAll(AddAmount);
		}
		ensure(Item->GetQuantity() == 1);
		AddItem(Item);
		return FItemAddResult::AddedAll(AddAmount);
	}

	//this should never happen as this is the client
	check(false);
	return FItemAddResult::AddedNone(-1,LOCTEXT("ErrorMessage", ""));
}

UBaseItem* UInventoryComponent::AddItem(UBaseItem* Item)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		//duplicate the item but set this owner as the new owner, ignoring the old item's owner
		UBaseItem* NewItem = NewObject<UBaseItem>(GetOwner(), Item->GetClass());
		NewItem->SetQuantity(Item->GetQuantity());
		NewItem->OwningInventory = this;
		NewItem->AddedToInventory(this);
		Items.Add(NewItem);

		// tell item to get replicated
		NewItem->MarkDirtyForReplication();

		return NewItem;
	}
	return nullptr;
}

#undef LOCTEXT_NAMESPACE
