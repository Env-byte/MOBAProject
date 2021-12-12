// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/AllMid/PSAllMid.h"

#include "Components/InventoryComponent.h"

APSAllMid::APSAllMid()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	InventoryComponent->SetCapacity(6);
}
