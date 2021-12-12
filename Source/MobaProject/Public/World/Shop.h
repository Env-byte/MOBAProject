// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shop.generated.h"

class UBaseItem;
class UInventoryComponent;
UCLASS()
class MOBAPROJECT_API AShop : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShop();

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UBaseItem>> ItemsInShop;

	UFUNCTION(BlueprintCallable)
	void ShowShop();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
