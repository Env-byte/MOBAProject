// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Shop.h"
// Sets default values
AShop::AShop()
{
}

void AShop::ShowShop()
{
}

// Called when the game starts or when spawned
void AShop::BeginPlay()
{
	Super::BeginPlay();
	for (const TSubclassOf<UBaseItem> Items : ItemsInShop)
	{
	}
}
