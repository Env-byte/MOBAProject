// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CHAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName)\
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS(NotBlueprintable)
class MOBAPROJECT_API UCHAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	UCHAttributeSet();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	void AdjustAttributeForMaxChange(
		const FGameplayAttributeData& AffectedAttribute,
		const FGameplayAttributeData& MaxAttribute,
		float NewMaxValue,
		const FGameplayAttribute& AffectedAttributeProperty
	);

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=On_RepHealth)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCHAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=On_RepMaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCHAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=On_RepMana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UCHAttributeSet, Mana)

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=On_RepMaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UCHAttributeSet, MaxMana)

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=On_RepAttackDamage)
	FGameplayAttributeData AttackDamage;
	ATTRIBUTE_ACCESSORS(UCHAttributeSet, AttackDamage)

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=On_RepMoveSpeed)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UCHAttributeSet, MoveSpeed)

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=On_RepAttackSpeed)
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UCHAttributeSet, AttackSpeed)

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=On_RepArmour)
	FGameplayAttributeData Armour;
	ATTRIBUTE_ACCESSORS(UCHAttributeSet, Armour)

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=On_RepAttackRange)
	FGameplayAttributeData AttackRange;
	ATTRIBUTE_ACCESSORS(UCHAttributeSet, AttackRange)

	float GetHealthPercent() const;
	float GetManaPercent() const;

protected:
	UFUNCTION()
	virtual void On_RepHealth(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void On_RepMaxHealth(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void On_RepMana(const FGameplayAttributeData& OldMana);

	UFUNCTION()
	virtual void On_RepMaxMana(const FGameplayAttributeData& OldMaxMana);

	UFUNCTION()
	virtual void On_RepAttackDamage(const FGameplayAttributeData& OldAttackDamage);

	UFUNCTION()
	virtual void On_RepMoveSpeed(const FGameplayAttributeData& OldMoveSpeed);
	
	UFUNCTION()
	virtual void On_RepAttackSpeed(const FGameplayAttributeData& OldAttackSpeed);
	
	UFUNCTION()
	virtual void On_RepArmour(const FGameplayAttributeData& OldArmour);
	
	UFUNCTION()
	virtual void On_RepAttackRange(const FGameplayAttributeData& OldAttackRange);
};
