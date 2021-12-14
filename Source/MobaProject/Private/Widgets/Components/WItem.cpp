// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/WItem.h"

void UWItem::SetItem(UBaseItem* InItem)
{
	Item = InItem;
	BP_OnItemChange();
}
