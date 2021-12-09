// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CHBase.generated.h"
struct FGameplayTagContainer;
DECLARE_LOG_CATEGORY_EXTERN(LogCHBase, Log, All);

class UCHAttributeSet;
class UCHAbilitySystemComponent;
class UGameplayEffect;

UCLASS()
class MOBAPROJECT_API ACHBase : public ACharacter, public IAbilitySystemInterface

{
	GENERATED_BODY()
	// Sets default values for this character's properties
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
	 * Get this Characters level to know what attribute values to spawn it with
	 */
	virtual int32 GetLevel();

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

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnDamaged(float Damage, const FHitResult HitInfo, const FGameplayTagContainer& DamageTags, ACHBase* InstigatorCharacter, AActor* DamageCauser);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	virtual void HandleDamage(float Damage, const FHitResult HitInfo, const FGameplayTagContainer& DamageTags, ACHBase* InstigatorCharacter, AActor* DamageCauser);
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	friend UCHAttributeSet;
	
	FORCEINLINE UCHAttributeSet* GetAttributeSet() const { return Attributes; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	////////// Ability System //////////
};
