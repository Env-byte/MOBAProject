// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WGameScoreboard.generated.h"


UCLASS(Abstract)
class MOBAPROJECT_API UWGameScoreboard : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWGameScoreboardItem* FriendlyScoreboardItem;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWGameScoreboardItem* EnemyScoreboardItem;
};
