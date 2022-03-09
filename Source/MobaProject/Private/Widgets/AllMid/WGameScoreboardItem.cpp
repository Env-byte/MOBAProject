// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AllMid/WGameScoreboardItem.h"

void UWGameScoreboardItem::SetPlayerState(APSAllMid* ThisPlayerState)
{
	PlayerState = ThisPlayerState;
	UpdateWidget();
}
