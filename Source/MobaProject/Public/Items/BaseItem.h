// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseItem.generated.h"
class UWItem;
class APSAllMid;
class ACHPlayable;
DECLARE_LOG_CATEGORY_EXTERN(LogBaseItem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemModified);

class UInventoryComponent;
class UWItemTooltip;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MOBAPROJECT_API UBaseItem : public UObject
{
	GENERATED_BODY()

	//This item instance's unique id
	UPROPERTY(ReplicatedUsing=OnRep_UniqueId)
	FGuid UniqueId;
public:
	UBaseItem();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGuid GetGuid() const { return UniqueId; }

	void SetGuid();

protected:
	////////// Basic Networking //////////
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

public:
	//Used to efficiently replicate inventory items
	UPROPERTY()
	int32 RepKey;

	//Mark the object as needing replication. We must call this internally after modifying any replicated properties
	void MarkDirtyForReplication();
	////////// Basic Networking //////////
	////////// Item //////////


	//The image icon for the item in inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item")
	UTexture2D* Thumbnail;

	//The display name for this item in inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item")
	FText DisplayName;

	//The description for this item in inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item")
	float ItemCost = 0;

	//The tooltip in the inventory for this item
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item")
	TSubclassOf<UWItemTooltip> ItemTooltip;

	//The inventory that owns this item
	UPROPERTY()
	UInventoryComponent* OwningInventory;

	//Whether this item can be stacked
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item")
	bool bStackable;

	//Item max stack size
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item", meta=(ClampMin=2, EditCondition=bStackable))
	int32 MaxStackSize;

	virtual void AddedToInventory(UInventoryComponent* Inventory);

	UPROPERTY(BlueprintAssignable)
	FOnItemModified OnItemModified;

	UFUNCTION(BlueprintCallable, Category="Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(BlueprintPure, Category="Item")
	FORCEINLINE int32 GetQuantity() const { return Quantity; }

	/**
	 * Only used on client. The slot number this item is in. 
	 */
	UPROPERTY(BlueprintReadWrite)
	UWItem* ItemSlotRef;

	/**
	 * Called on server straight after the player buys an item
	 */
	virtual void OnBuy(ACHPlayable* PlayerCharacter);
	/**
	 * Called on server straight after the player sells an item
	 */
	virtual void OnSell(ACHPlayable* PlayerCharacter, APSAllMid* PlayerState);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetSellPrice() const { return static_cast<float>(static_cast<int>(ItemCost * .6)); };

protected:
	//the quantity of this item
	UPROPERTY(ReplicatedUsing=OnRep_Quantity, EditAnywhere, Category="Item", meta=(UIMin=1, EditCondition=bStackable))
	int32 Quantity;

	UFUNCTION()
	void OnRep_Quantity();
	UFUNCTION()
	void OnRep_UniqueId();
};
