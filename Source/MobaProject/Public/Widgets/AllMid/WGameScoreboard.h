// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WGameScoreboard.generated.h"

class APSAllMid;
class UWGameScoreboardItem;

UCLASS(Abstract)
class MOBAPROJECT_API UWGameScoreboard : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite)
	TMap<APSAllMid*, UWGameScoreboardItem*> FriendlyScoreboardItems;

	UPROPERTY(BlueprintReadWrite)
	TMap<APSAllMid*, UWGameScoreboardItem*> EnemyScoreboardItems;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnAddFriendly(UWGameScoreboardItem* ScoreboardItem);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnAddEnemy(UWGameScoreboardItem* ScoreboardItem);


public:
	void AddFriendlyItem(APSAllMid* PlayerState, UWGameScoreboardItem* ScoreboardItem);
	void AddEnemyItem(APSAllMid* PlayerState, UWGameScoreboardItem* ScoreboardItem);

	UWGameScoreboardItem* GetScoreboardItem(const APSAllMid* PlayerState);
};
