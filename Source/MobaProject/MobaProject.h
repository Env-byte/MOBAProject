// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMobaProject, Log, All);

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	PrimaryAttack,
	Ability1,
	Ability2,
	Ability3
};

UENUM(BlueprintType)
enum class ETeam : uint8
{
	NeutralTeam,
	RedTeam,
	BlueTeam
};