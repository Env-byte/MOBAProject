// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "PSAttributeSet.generated.h"
class APSAllMid;
DECLARE_LOG_CATEGORY_EXTERN(LogPSAttributeSet, Log, All);

#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName)\
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
class ACHBase;
/**
 * 
 */
UCLASS(NotBlueprintable)
class MOBAPROJECT_API UPSAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	UPSAttributeSet();

	APSAllMid* GetOwningActor();

	UPROPERTY()
	APSAllMid* PlayerStateRef;
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=On_RepCharacterLevel)
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(UPSAttributeSet, CharacterLevel)

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=On_RepCharacterExperience)
	FGameplayAttributeData CharacterExperience;
	ATTRIBUTE_ACCESSORS(UPSAttributeSet, CharacterExperience)

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=On_RepGold)
	FGameplayAttributeData Gold;
	ATTRIBUTE_ACCESSORS(UPSAttributeSet, Gold)
protected:
	UFUNCTION()
	virtual void On_RepCharacterLevel(const FGameplayAttributeData& OldCharacterLevel);
	UFUNCTION()
	virtual void On_RepCharacterExperience(const FGameplayAttributeData& OldCharacterExperience);

	UFUNCTION()
	virtual void On_RepGold(const FGameplayAttributeData& OldGold);
};
