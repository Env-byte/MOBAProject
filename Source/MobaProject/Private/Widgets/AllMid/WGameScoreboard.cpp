// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AllMid/WGameScoreboard.h"

#include "Widgets/AllMid/WGameScoreboardItem.h"

void UWGameScoreboard::AddFriendlyItem(APSAllMid* PlayerState, UWGameScoreboardItem* ScoreboardItem)
{
	FriendlyScoreboardItems.Add(PlayerState, ScoreboardItem);
	BP_OnAddFriendly(ScoreboardItem);
}

void UWGameScoreboard::AddEnemyItem(APSAllMid* PlayerState, UWGameScoreboardItem* ScoreboardItem)
{
	EnemyScoreboardItems.Add(PlayerState, ScoreboardItem);
	BP_OnAddEnemy(ScoreboardItem);
}

UWGameScoreboardItem* UWGameScoreboard::GetScoreboardItem(const APSAllMid* PlayerState)
{
	UWGameScoreboardItem** ItemPtr = FriendlyScoreboardItems.Find(PlayerState);
	if (ItemPtr)
	{
		return *ItemPtr;
	}
	ItemPtr = EnemyScoreboardItems.Find(PlayerState);
	if (ItemPtr)
	{
		return *ItemPtr;
	}
	return nullptr;
}
