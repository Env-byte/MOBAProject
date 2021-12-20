// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WItem.generated.h"
class UBaseItem;
/**
 * 
 */
UCLASS()
class MOBAPROJECT_API UWItem : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="Inventory Item Widget")
	UBaseItem* Item;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnItemChange();
public:
	/**
	 * Set a new item
	 */
	UFUNCTION(BlueprintCallable)
	void SetItem(UBaseItem* InItem);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UBaseItem* GetItem() const { return Item; };
};
