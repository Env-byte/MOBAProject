// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/AllMid/PSAllMid.h"

#include "Components/InventoryComponent.h"
#include "Framework/AllMid/PCAllMid.h"

void APSAllMid::BeginPlay()
{
	Super::BeginPlay();
	
	APCAllMid* PC = GetOwner<APCAllMid>();
	if (PC)
	{
		if (PC->IsLocalController())
		{
			PC->PlayerStateReady(this);
		}
	}
}

APSAllMid::APSAllMid()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	InventoryComponent->SetCapacity(6);
}
