// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MobaProject/MobaProject.h"
#include "PlayerSpawn.generated.h"

UCLASS()
class MOBAPROJECT_API APlayerSpawn : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	APlayerSpawn();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETeam Team;
};
