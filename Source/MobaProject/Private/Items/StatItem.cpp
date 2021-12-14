// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/StatItem.h"

#include "Characters/Playable/CHPlayable.h"
DEFINE_LOG_CATEGORY(LogStatItem);

void UStatItem::ApplyStats(const ACHPlayable* PlayerCharacter)
{
	UAbilitySystemComponent* AbilitySystemComponent = PlayerCharacter->GetAbilitySystemComponent();
	if (!AbilitySystemComponent) { return; }
	if (!ItemStats) { return; }
	UE_LOG(LogStatItem, Display, TEXT("%s ApplyStats"), *this->GetName())

	const FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ItemStats, 0, ContextHandle);

	if (SpecHandle.IsValid())
	{
		StatHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void UStatItem::RemoveStats(const ACHPlayable* PlayerCharacter) const
{
	UAbilitySystemComponent* AbilitySystemComponent = PlayerCharacter->GetAbilitySystemComponent();
	if (!AbilitySystemComponent) { return; }
	if (!StatHandle.IsValid()) { return; }
	UE_LOG(LogStatItem, Display, TEXT("%s RemoveStats"), *this->GetName())

	AbilitySystemComponent->RemoveActiveGameplayEffect(StatHandle);
}

void UStatItem::OnBuy(ACHPlayable* PlayerCharacter)
{
	Super::OnBuy(PlayerCharacter);
	if (!PlayerCharacter->HasAuthority()) { return; }
	ApplyStats(PlayerCharacter);
}

void UStatItem::OnSell(ACHPlayable* PlayerCharacter)
{
	Super::OnSell(PlayerCharacter);
	if (!PlayerCharacter->HasAuthority()) { return; }
	RemoveStats(PlayerCharacter);
}
