// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CharacterNamePlate.generated.h"

class UWNamePlate;
/**
 * 
 */
UCLASS()
class MOBAPROJECT_API UCharacterNamePlate : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UCharacterNamePlate();

	UWNamePlate* GetNamePlateWidget() const;
};
