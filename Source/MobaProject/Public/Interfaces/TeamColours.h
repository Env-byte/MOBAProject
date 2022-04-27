// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaProject/MobaProject.h"
#include "UObject/Interface.h"
#include "TeamColours.generated.h"
DECLARE_LOG_CATEGORY_EXTERN(LogTeamColours, Log, All);

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UTeamColours : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MOBAPROJECT_API ITeamColours
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * ClientTeam is the local players team
	 */
	virtual FVector GetColour(ETeam ClientTeam);

	/**
	 * Get this objects team
	 */
	virtual ETeam GetTeam();

	/**
	 * Set this objects colour
	 */
	UFUNCTION(BlueprintImplementableEvent)
	 void SetColour(FVector Colour);

	/**
	 * Gets the clients team
	 */
	virtual ETeam GetClientTeam(UWorld* World);
};
