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

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_CharacterLevel)
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(UPSAttributeSet, CharacterLevel)

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_CharacterExperience)
	FGameplayAttributeData CharacterExperience;
	ATTRIBUTE_ACCESSORS(UPSAttributeSet, CharacterExperience)

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_Gold)
	FGameplayAttributeData Gold;
	ATTRIBUTE_ACCESSORS(UPSAttributeSet, Gold)

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_PlayersKilled)
	FGameplayAttributeData PlayersKilled;
	ATTRIBUTE_ACCESSORS(UPSAttributeSet, PlayersKilled)

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_Deaths)
	FGameplayAttributeData Deaths;
	ATTRIBUTE_ACCESSORS(UPSAttributeSet, Deaths)
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_MinionsKilled)
	FGameplayAttributeData MinionsKilled;
	ATTRIBUTE_ACCESSORS(UPSAttributeSet, MinionsKilled)

protected:
	UFUNCTION()
	virtual void OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel);
	UFUNCTION()
	virtual void OnRep_CharacterExperience(const FGameplayAttributeData& OldCharacterExperience);
	UFUNCTION()
	virtual void OnRep_Gold(const FGameplayAttributeData& OldGold);
	UFUNCTION()
	virtual void OnRep_PlayersKilled(const FGameplayAttributeData& OldKills);
	UFUNCTION()
	virtual void OnRep_Deaths(const FGameplayAttributeData& OldDeaths);
	UFUNCTION()
	virtual void OnRep_MinionsKilled(const FGameplayAttributeData& OldMinionsKilled);
};
