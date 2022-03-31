// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/CanTakeDamage.h"


void ICanTakeDamage::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
}

FActorHelper ICanTakeDamage::GetActorInfo()
{
	return FActorHelper{};
}

UCHAttributeSet* ICanTakeDamage::GetAttributeSet()
{
	UE_LOG(LogTemp, Warning, TEXT("Default Implementation Run: %s"), *GetActorInfo().Actor->GetClass()->GetName())
	return nullptr;
}

FName ICanTakeDamage::GetEntityName()
{
	return FName("Not Implemented");
}

UCHAbilitySystemComponent* ICanTakeDamage::GetAbilitySystem_Implementation()
{
	return nullptr;
}
