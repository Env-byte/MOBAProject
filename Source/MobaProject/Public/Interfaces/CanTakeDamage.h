// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MobaProject/MobaProject.h"
#include "UObject/Interface.h"
#include "CanTakeDamage.generated.h"


class ACHPlayable;
class UCHAttributeSet;
class UCHAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FActorHelper
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETeam Team;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Actor;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, Blueprintable)
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
	virtual void HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags,ACHPlayable* SourcePlayer);

	virtual FActorHelper GetActorInfo();

	virtual UCHAttributeSet* GetAttributeSet();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UCHAbilitySystemComponent* GetAbilitySystem();

	virtual FName GetEntityName();

	virtual UCHAbilitySystemComponent* GetAbilitySystem_Implementation();
};
