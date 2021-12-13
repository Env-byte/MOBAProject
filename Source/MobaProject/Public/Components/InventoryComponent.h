// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UBaseItem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, const TArray<UBaseItem*>&, Items);

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemsAdded UMETA(DisplayName="No items added"),
	IAR_SomeItemsAdded UMETA(DisplayName="Some items added"),
	IAR_AllItemsAdded UMETA(DisplayName="All items added")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

public:
	FItemAddResult(): AmountToGive(0), ActualAmountGiven(0), Result()
	{
	}


	FItemAddResult(int32 InItemQuantity) : AmountToGive(InItemQuantity), ActualAmountGiven(0), Result()
	{
	}

	FItemAddResult(int32 InItemQuantity, int32 InQuantityAdded) : AmountToGive(InItemQuantity), ActualAmountGiven(InQuantityAdded), Result()
	{
	}

	//The amount of the item that we tried to add
	UPROPERTY(BlueprintReadOnly, Category="Item Add Result")
	int32 AmountToGive;

	//The amount of the item that was actually added in the end.
	//Maybe we tried adding 10 items but only 8 could be added because of capacity / weight.
	UPROPERTY(BlueprintReadOnly, Category="Item Add Result")
	int32 ActualAmountGiven;

	//The result 
	UPROPERTY(BlueprintReadOnly, Category="Item Add Result")
	EItemAddResult Result;

	//If something went wrong, like we didnt have enough capacity or carrying too much weight this contains the reason why
	UPROPERTY(BlueprintReadOnly, Category="Item Add Result")
	FText ErrorText;

	//Helpers
	static FItemAddResult AddedNone(const int32 InItemQuantity, const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult(InItemQuantity);
		AddedNoneResult.Result = EItemAddResult::IAR_NoItemsAdded;
		AddedNoneResult.ErrorText = ErrorText;
		return AddedNoneResult;
	}

	static FItemAddResult AddedSome(const int32 InItemQuantity, const int32 ActualAmountGiven, const FText& ErrorText)
	{
		FItemAddResult AddedSomeResult(InItemQuantity, ActualAmountGiven);
		AddedSomeResult.Result = EItemAddResult::IAR_SomeItemsAdded;
		AddedSomeResult.ErrorText = ErrorText;

		return AddedSomeResult;
	}

	static FItemAddResult AddedAll(const int32 InItemQuantity)
	{
		FItemAddResult AddAllResult(InItemQuantity, InItemQuantity);
		AddAllResult.Result = EItemAddResult::IAR_AllItemsAdded;
		return AddAllResult;
	}

	FString ToString() const
	{
		const FString ResultText = UEnum::GetDisplayValueAsText(Result).ToString();
		if (Result == EItemAddResult::IAR_AllItemsAdded)
		{
			return FString::Printf(TEXT("Result: %s. AmountToGive: %d. ActualAmountGiven: %d"), *ResultText, AmountToGive, ActualAmountGiven);
		}
		return FString::Printf(TEXT("Result: %s. Error:%s. AmountToGive: %d. ActualAmountGiven: %d"), *ResultText, *ErrorText.ToString(), AmountToGive, ActualAmountGiven);
	}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MOBAPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	////////// Basic Networking //////////
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	////////// Basic Networking //////////
public:
	// Sets default values for this component's properties
	UInventoryComponent();

	/**
	* Add an item to the inventory.
	*/
	UFUNCTION(BlueprintCallable, Category="Inventory")
	FItemAddResult TryAddItem(UBaseItem* Item);

	/**
	* Add an item to the inventory using the item class instead of an item instance.
	*/
	UFUNCTION(BlueprintCallable, Category="Inventory")
	FItemAddResult TryAddItemFromClass(TSubclassOf<UBaseItem> ItemClass, const int32 Quantity);

	/**
	 * Take some quantity away from the item
	 */
	UFUNCTION(BlueprintCallable, Category="Inventory")
	int32 ConsumeItem(UBaseItem* Item, int32 Quantity);

	//Remove the item from inventory
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool RemoveItem(UBaseItem* Item);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	UBaseItem* FindItem(UBaseItem* Item) const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void SetCapacity(const int32 NewCapacity);

	UFUNCTION(BlueprintPure, Category="Inventory")
	FORCEINLINE int32 GetCapacity() const { return Capacity; }

	UFUNCTION(BlueprintPure, Category="Inventory")
	FORCEINLINE TArray<UBaseItem*> GetItems() const { return Items; }

	UFUNCTION(Client, Reliable)
	void Client_RefreshInventory();

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	//Dont Call Items.Add() directly, use this function instead, as it handles replication and ownership
	UBaseItem* AddItem(UBaseItem* Item);

	UPROPERTY()
	int32 ReplicatedItemsKey;
protected:
	//The maximum number of items the inventory can hold. For Players, backpacks and other items increase this limit
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory", meta=(ClampMin=0, ClampMax=200))
	int32 Capacity;

	UPROPERTY(ReplicatedUsing=OnRep_Items, VisibleAnywhere, Category="Inventory")
	TArray<UBaseItem*> Items;
private:
	UFUNCTION()
	void OnRep_Items();
	
	FItemAddResult TryAddItem_Internal(UBaseItem* Item);
};
