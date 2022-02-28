﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CHBase.h"
#include "Interfaces/TeamColours.h"
#include "CHNonPlayable.generated.h"

/**
 * 
 */
UCLASS(Abstract, NotBlueprintable)
class MOBAPROJECT_API ACHNonPlayable : public ACHBase, public ITeamColours
{
	GENERATED_BODY()
protected:
	/**
	 * Non playable characters should not display there level
	 */
	virtual void SetupNamePlateWidget() override;

public:
	/**
	 * Get the team for this object
	 */
	virtual ETeam GetTeam() override;

	virtual void BeginPlay() override;
};
