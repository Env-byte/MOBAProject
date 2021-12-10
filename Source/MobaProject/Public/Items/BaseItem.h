// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemModified);

class UInventoryComponent;
class ACHBase;
class UWItemTooltip;

/**
 * 
 */
UCLASS(Abstract)
class MOBAPROJECT_API UBaseItem : public UObject
{
	GENERATED_BODY()
public:
	UBaseItem();

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

	//Whether this item can be stacked
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item")
	bool bStackable;

	//Item max stack size
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item", meta=(ClampMin=2, EditCondition=bStackable))
	int32 MaxStackSize;

	//The tooltip in the inventory for this item
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item")
	TSubclassOf<UWItemTooltip> ItemTooltip;

	//the quantity of this item
	UPROPERTY(ReplicatedUsing=OnRep_Quantity, EditAnywhere, Category="Item", meta=(UIMin=1, EditCondition=bStackable))
	int32 Quantity;

	//The inventory that owns this item
	UPROPERTY()
	UInventoryComponent* OwningInventory;

	UPROPERTY(BlueprintAssignable)
	FOnItemModified OnItemModified;

	UFUNCTION(BlueprintCallable, Category="Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(BlueprintPure, Category="Item")
	FORCEINLINE int32 GetQuantity() const { return Quantity; }

	virtual void Use(ACHBase* Character);
	virtual void AddedToInventory(UInventoryComponent* Inventory);

	UFUNCTION()
	void OnRep_Quantity();
};
