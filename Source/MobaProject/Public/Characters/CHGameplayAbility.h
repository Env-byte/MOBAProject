// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MobaProject/MobaProject.h"
#include "CHGameplayAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MOBAPROJECT_API UCHGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	/**
	 * The action id to trigger this
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EAbilityInputID AbilityInputID = EAbilityInputID::None;
};
