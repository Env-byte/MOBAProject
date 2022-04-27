// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MobaProject/MobaProject.h"
#include "GSAllMid.generated.h"
DECLARE_LOG_CATEGORY_EXTERN(LogGSAllMid, Log, All);

/**
 * 
 */
UCLASS(Abstract)
class MOBAPROJECT_API AGSAllMid : public AGameState
{
	GENERATED_BODY()

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_TowersDestroyed)
	int32 RedTowersDestroyed = 0;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_TowersDestroyed)
	int32 BlueTowersDestroyed = 0;

	UFUNCTION()
	void OnRep_TowersDestroyed();
public:
	void IncrementDestroyedTower(ETeam Team);

	virtual void BeginPlay() override;
};
