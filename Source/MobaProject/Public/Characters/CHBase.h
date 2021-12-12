// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CHAbilitySystemComponent.h"
#include "CHBase.generated.h"
class UCharacterNamePlate;
struct FGameplayTagContainer;
DECLARE_LOG_CATEGORY_EXTERN(LogCHBase, Log, All);

class UCHAttributeSet;
class UGameplayEffect;

UCLASS(Abstract, NotBlueprintable)
class MOBAPROJECT_API ACHBase : public ACharacter, public IAbilitySystemInterface

{
	GENERATED_BODY()
public:
	ACHBase();

	
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	////////// Ability System //////////
	/**
	 * AbilitySystemComponent for this Character
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ability", meta=(AllowPrivateAccess=true))
	UCHAbilitySystemComponent* AbilitySystemComponent;

	/**
	 * The Attribute set for this Character
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ability", meta=(AllowPrivateAccess=true))
	UCHAttributeSet* Attributes;

	/**
	 * This Characters default attributes for when its spawned or levels up
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Attribute")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	/**
	 * Name plate component to show character health / mana level and entity name
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCharacterNamePlate* NamePlateComponent;

	/**
	 * Used to set the NamePlateComponent Name. Ex PlayerName, or for Ai 'Minion' or 'Turret'
	 */
	virtual FName GetEntityName();

	/**
	 * Get this Characters level to know what attribute values to spawn it with
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual int32 GetCharacterLevel() const;

private:
	/**
	 * Initialize this characters attributes and owner
	 */
	virtual void InitializeAttributes();
	virtual void InitializeOwningActor();
	virtual void InitializeAbilityBinds();
public:
	/**
	 * This only happens on server 
	 */
	virtual void PossessedBy(AController* NewController) override;

	/**
	 * This happens client side
	 */
	virtual void OnRep_PlayerState() override;

	////////// Server only  //////////
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnDamaged(float Damage, const FHitResult HitInfo, const FGameplayTagContainer& DamageTags, ACHBase* InstigatorCharacter, AActor* DamageCauser);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleDamage(float Damage, const FHitResult HitInfo, const FGameplayTagContainer& DamageTags, ACHBase* InstigatorCharacter, AActor* DamageCauser);
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags) ;
	////////// Server only //////////

	/**
	* Client only Attribute On_Rep
	*/
	virtual void OnRep_Attribute(const FGameplayAttribute& Attribute, const FGameplayAttributeData& OldValue, const FGameplayAttributeData& NewValue);

	friend UCHAttributeSet;

	FORCEINLINE UCHAttributeSet* GetAttributeSet() const { return Attributes; }
	FORCEINLINE UCharacterNamePlate* GetNamePlateComponent() const { return NamePlateComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	////////// Ability System //////////

	virtual void BeginPlay() override;
};
