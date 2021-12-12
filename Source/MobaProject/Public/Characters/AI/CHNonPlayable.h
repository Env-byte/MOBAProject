// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CHBase.h"
#include "CHNonPlayable.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MOBAPROJECT_API ACHNonPlayable : public ACHBase
{
	GENERATED_BODY()
protected:
	/**
	 * Non playable characters should not display there level
	 */
	virtual void SetupNamePlateWidget() override;
};
