// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/CanTakeDamage.h"


void ICanTakeDamage::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
}

FActorHelper ICanTakeDamage::GetActorInfo()
{
	return FActorHelper{};
}