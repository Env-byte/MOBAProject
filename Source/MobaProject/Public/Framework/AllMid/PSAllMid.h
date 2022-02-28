// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "PCAllMid.h"
#include "GameFramework/PlayerState.h"
#include "MobaProject/MobaProject.h"
#include "PSAllMid.generated.h"
DECLARE_LOG_CATEGORY_EXTERN(LogPSAllMid, Log, All);

class UGameplayEffect;
class UPSAttributeSet;
class UPSAbilitySystemComponent;
class UInventoryComponent;
/**
 * 
 */
UCLASS(Abstract)
class MOBAPROJECT_API APSAllMid : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
protected:
	APSAllMid();

	virtual void BeginPlay() override;

	////////// Ability System //////////
	/**
	 * AbilitySystemComponent for this Character
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ability", meta=(AllowPrivateAccess=true))
	UPSAbilitySystemComponent* AbilitySystemComponent;

	/**
	 * The Attribute set for this Character
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ability", meta=(AllowPrivateAccess=true))
	UPSAttributeSet* Attributes;

	/**
	 * This players passive gold income effect
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Attribute")
	TSubclassOf<UGameplayEffect> GoldPassiveEffect;

	/**
	 * This players default attributes
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Attribute")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	UFUNCTION()
	void InitializeAttributes();

	UFUNCTION(Client, Reliable)
		void Client_UseTeamColours(ETeam ThisTeam);


	UFUNCTION()
	void OnPlayerInventoryUpdated(const TArray<UBaseItem*>& Items);
public:
	void UseTeamColours(ETeam ThisTeam);


	UPROPERTY()
	APCAllMid* PlayerControllerRef;

	FORCEINLINE UPSAttributeSet* GetAttributeSet() const { return Attributes; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	* Client only Attribute On_Rep
	*/
	void OnRep_Attribute(const FGameplayAttribute& Attribute, const FGameplayAttributeData& OldValue,
	                     const FGameplayAttributeData& NewValue);
	////////// Ability System //////////

	UPROPERTY(BlueprintReadOnly)
	UInventoryComponent* InventoryComponent;

	/**
	 * The team this player is on
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Team)
	ETeam Team = ETeam::NeutralTeam;

	/**
	 * We want to update all minions turrets in game to use correct colour here
	 */
	UFUNCTION()
	void OnRep_Team();
};
