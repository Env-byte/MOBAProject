// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WItemsShop.generated.h"

class UBaseItem;
/**
 * 
 */
UCLASS()
class MOBAPROJECT_API UWItemsShop : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	UPROPERTY(EditDefaultsOnly, Category="Items")
	TArray<TSubclassOf<UBaseItem>> ItemsInShop;
public:
	UFUNCTION(BlueprintImplementableEvent)
	void InitShop(const TArray<TSubclassOf<UBaseItem>>& ItemClasses);
};
