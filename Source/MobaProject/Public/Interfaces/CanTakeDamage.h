// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CanTakeDamage.generated.h"
DECLARE_LOG_CATEGORY_EXTERN(LogTeamColours, Log, All);

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCanTakeDamage : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MOBAPROJECT_API ICanTakeDamage
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags);
};
