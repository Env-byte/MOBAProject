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

void UPSAttributeSet::OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPSAttributeSet, CharacterLevel, OldCharacterLevel)
	APSAllMid* PlayerState = GetOwningActor();
	if (PlayerState)
	{
		PlayerState->OnRep_Attribute(GetCharacterLevelAttribute(), OldCharacterLevel, CharacterLevel);
	}
}

void UPSAttributeSet::OnRep_CharacterExperience(const FGameplayAttributeData& OldCharacterExperience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPSAttributeSet, CharacterExperience, OldCharacterExperience)
	APSAllMid* PlayerState = GetOwningActor();
	if (PlayerState)
	{
		PlayerState->OnRep_Attribute(GetCharacterExperienceAttribute(), OldCharacterExperience, CharacterExperience);
	}
}

void UPSAttributeSet::OnRep_Gold(const FGameplayAttributeData& OldGold)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPSAttributeSet, Gold, OldGold)
	APSAllMid* PlayerState = GetOwningActor();
	if (PlayerState)
	{
		PlayerState->OnRep_Attribute(GetGoldAttribute(), OldGold, Gold);
	}
}

void UPSAttributeSet::OnRep_PlayersKilled(const FGameplayAttributeData& OldKills)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPSAttributeSet, PlayersKilled, OldKills)
	APSAllMid* PlayerState = GetOwningActor();
	if (PlayerState)
	{
		PlayerState->OnRep_Attribute(GetPlayersKilledAttribute(), OldKills, PlayersKilled);
	}
}

void UPSAttributeSet::OnRep_Deaths(const FGameplayAttributeData& OldDeaths)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPSAttributeSet, Deaths, OldDeaths)
	APSAllMid* PlayerState = GetOwningActor();
	if (PlayerState)
	{
		PlayerState->OnRep_Attribute(GetDeathsAttribute(), OldDeaths, Deaths);
	}
}

void UPSAttributeSet::OnRep_MinionsKilled(const FGameplayAttributeData& OldMinionsKilled)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPSAttributeSet, MinionsKilled, OldMinionsKilled)
	APSAllMid* PlayerState = GetOwningActor();
	if (PlayerState)
	{
		PlayerState->OnRep_Attribute(GetMinionsKilledAttribute(), OldMinionsKilled, MinionsKilled);
	}
}
