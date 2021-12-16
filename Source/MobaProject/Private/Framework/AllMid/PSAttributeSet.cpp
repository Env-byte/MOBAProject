// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/AllMid/PSAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Characters/CHBase.h"
#include "Framework/AllMid/PSAllMid.h"
#include "Net/UnrealNetwork.h"
DEFINE_LOG_CATEGORY(LogPSAttributeSet);

UPSAttributeSet::UPSAttributeSet()
{
}

APSAllMid* UPSAttributeSet::GetOwningActor()
{
	if (PlayerStateRef)
	{
		return PlayerStateRef;
	}

	AActor* OwningActor = GetOwningAbilitySystemComponent()->GetAvatarActor();
	if (OwningActor)
	{
		PlayerStateRef = Cast<APSAllMid>(OwningActor);
		return PlayerStateRef;
	}
	return nullptr;
}

void UPSAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UPSAttributeSet, CharacterLevel, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UPSAttributeSet, CharacterExperience, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UPSAttributeSet, Gold, COND_OwnerOnly, REPNOTIFY_Always)
}

// this only happens on server
void UPSAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	const FGameplayEffectContextHandle ContextHandle = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = ContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	float DeltaValue{0.f};
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		DeltaValue = Data.EvaluatedData.Magnitude;
	}
}

void UPSAttributeSet::On_RepCharacterLevel(const FGameplayAttributeData& OldCharacterLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPSAttributeSet, CharacterLevel, OldCharacterLevel)
	APSAllMid* PlayerState = GetOwningActor();
	if (PlayerState)
	{
		PlayerState->OnRep_Attribute(GetCharacterLevelAttribute(), OldCharacterLevel, CharacterLevel);
	}
}

void UPSAttributeSet::On_RepCharacterExperience(const FGameplayAttributeData& OldCharacterExperience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPSAttributeSet, CharacterExperience, OldCharacterExperience)
	APSAllMid* PlayerState = GetOwningActor();
	if (PlayerState)
	{
		PlayerState->OnRep_Attribute(GetCharacterExperienceAttribute(), OldCharacterExperience, CharacterExperience);
	}
}

void UPSAttributeSet::On_RepGold(const FGameplayAttributeData& OldGold)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPSAttributeSet, Gold, OldGold)
	APSAllMid* PlayerState = GetOwningActor();
	if (PlayerState)
	{
		PlayerState->OnRep_Attribute(GetGoldAttribute(), OldGold, Gold);
	}
}
