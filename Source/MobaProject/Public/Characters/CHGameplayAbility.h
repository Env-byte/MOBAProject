// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MobaProject/MobaProject.h"
#include "Playable/CHPlayable.h"
#include "CHGameplayAbility.generated.h"

class APCAllMid;
class UWPlayerHud;
/**
 * 
 */
UCLASS(Abstract)
class MOBAPROJECT_API UCHGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	UCHGameplayAbility();
protected:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UGameplayEffect* BP_GetCooldownGameplayEffect() const { return GetCooldownGameplayEffect(); };

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	UWPlayerHud* GetPlayerHudWidget();

	UFUNCTION(BlueprintCallable)
	void StartCooldown();

	UFUNCTION(BlueprintCallable)
	FRotator GetCharacterToMouseRotation();

	/**
	 * The character that used this ability
	 */
	UPROPERTY(BlueprintReadWrite)
	ACHBase* CharacterRef;

	/**
	 * The player controller ref for the character using this ability
	 */
	UPROPERTY(BlueprintReadWrite)
	APCAllMid* PlayerControllerRef;

	/**
	 * Rotate character on z axis. Using simple rotate but could implement something with animations
	 */
	UFUNCTION(BlueprintCallable)
	void RotateCharacter(const float Z);
public:
	/**
	 * The curve table containing this abilities information
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCurveTable* AbilityDataTable;

	/**
	 * The action id to trigger this Ability
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EAbilityInputID AbilityInputID = EAbilityInputID::None;

	/**
	 * The Abilities icon
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* AbilityIcon;

	/**
	 * The Abilities name
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AbilityName;
};
