// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ConsumableItem.h"

#include "GameplayEffect.h"
#include "Characters/Playable/CHPlayable.h"
DEFINE_LOG_CATEGORY(LogConsumableItem);

bool UConsumableItem::ConsumeItem(const ACHPlayable* Character, float& OutDuration)
{
	UAbilitySystemComponent* AbilitySystemComponent = Character->GetAbilitySystemComponent();
	if (!AbilitySystemComponent) { return false; }
	if (!ConsumableEffect) { return false; }
	const FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ConsumableEffect, 0, ContextHandle);
	if (SpecHandle.IsValid())
	{
		ConsumableHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		const FActiveGameplayEffect* ActiveEffect = AbilitySystemComponent->GetActiveGameplayEffect(ConsumableHandle);
		if (ActiveEffect)
		{
			ConsumableEffect.GetDefaultObject()->DurationMagnitude.AttemptCalculateMagnitude(ActiveEffect->Spec, OutDuration);
			return true;
		}
	}
	return false;
}

void UConsumableItem::Use(ACHPlayable* Character)
{
	if (!Character->HasAuthority()) { return; }
	float Duration = 0.f;
	UE_LOG(LogConsumableItem, Display, TEXT("%s Use"), *this->GetName())
	if (ConsumeItem(Character, Duration))
	{
		BP_OnUse(Character, Duration);
	}
}
