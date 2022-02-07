// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/AI/CHMinion.h"
#include "GameFramework/Actor.h"
#include "MobaProject/MobaProject.h"
#include "MinionSpawner.generated.h"

UCLASS()
class MOBAPROJECT_API AMinionSpawner : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AMinionSpawner();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:

	void Spawn();
	
	/**
	 * How often to spawn the minions 
	 */
	UPROPERTY(EditAnywhere, Category=Spanwer)
	float SpawnFrequency;

	/**
	 * How many minions to spawn per wave
	 */
	UPROPERTY(EditAnywhere, Category=Spanwer)
	int32 UnitsPerWave = 5;

	/**
	 * The minion to spawn
	 */
	UPROPERTY(EditAnywhere, Category=Spanwer)
	TSubclassOf<ACHMinion> MinionClass;

	/**
	 * The team the spawned minions should be set to
	 */
	UPROPERTY(BlueprintReadWrite)
	ETeam Team;
};
