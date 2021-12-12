// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ConsumableItem.h"

void UConsumableItem::Use(ACHPlayable* Character)
{
	BP_OnUse(Character);
}

