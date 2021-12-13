// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CHAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Characters/CHBase.h"
#include "Net/UnrealNetwork.h"
DEFINE_LOG_CATEGORY(LogCHAttributeSet);

UCHAttributeSet::UCHAttributeSet()
{
}

ACHBase* UCHAttributeSet::GetOwningActor()
{
	if (CharacterBaseRef)
	{
		return CharacterBaseRef;
	}

	AActor* OwningActor = GetOwningAbilitySystemComponent()->GetAvatarActor();
	if (OwningActor)
	{
		CharacterBaseRef = Cast<ACHBase>(OwningActor);
		return CharacterBaseRef;
	}
	return nullptr;
}

void UCHAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UCHAttributeSet, Health, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UCHAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UCHAttributeSet, Mana, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UCHAttributeSet, MaxMana, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UCHAttributeSet, AttackDamage, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UCHAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UCHAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UCHAttributeSet, AttackRange, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UCHAttributeSet, Armour, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UCHAttributeSet, Gold, COND_None, REPNOTIFY_Always)
}

void UCHAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	//if max health changes or max mana scale current health up/down
	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	if (Attribute == GetManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
}

// this only happens on server
void UCHAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
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

	ACHBase* TargetCharacter{nullptr};
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetCharacter = Cast<ACHBase>(TargetActor);
	}

	//notify character that health has changed. Also prevent health going below 0 or exceeding max
	//make sure to set MaxHealth Before Health when init attributes
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		if (TargetCharacter)
		{
			TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		//prevent mana going below 0 or exceeding max
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
}

void UCHAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilitySystemComponent = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilitySystemComponent)
	{
		const float Difference = NewMaxValue - CurrentMaxValue;
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		const float NewDelta = Difference + CurrentValue;
		AbilitySystemComponent->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

float UCHAttributeSet::GetHealthPercent() const
{
	if (GetHealth() == 0.f)
	{
		return 0.f;
	}
	return GetHealth() / GetMaxHealth();
}

float UCHAttributeSet::GetManaPercent() const
{
	if (GetMana() == 0.f)
	{
		return 0.f;
	}
	return GetMana() / GetMaxMana();
}

void UCHAttributeSet::On_RepHealth(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, Health, OldHealth)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetHealthAttribute(), OldHealth, Health);
	}
}

void UCHAttributeSet::On_RepMaxHealth(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, MaxHealth, OldHealth)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetMaxHealthAttribute(), OldHealth, MaxHealth);
	}
}

void UCHAttributeSet::On_RepMana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, Mana, OldMana)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetManaAttribute(), OldMana, Mana);
	}
}

void UCHAttributeSet::On_RepMaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, MaxMana, OldMaxMana)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetMaxManaAttribute(), OldMaxMana, MaxMana);
	}
}

void UCHAttributeSet::On_RepAttackDamage(const FGameplayAttributeData& OldAttackDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, AttackDamage, OldAttackDamage)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetAttackDamageAttribute(), OldAttackDamage, AttackDamage);
	}
}

void UCHAttributeSet::On_RepMoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, MoveSpeed, OldMoveSpeed)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetMoveSpeedAttribute(), OldMoveSpeed, MoveSpeed);
	}
}

void UCHAttributeSet::On_RepAttackSpeed(const FGameplayAttributeData& OldAttackSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, AttackSpeed, OldAttackSpeed)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetAttackSpeedAttribute(), OldAttackSpeed, AttackSpeed);
	}
}

void UCHAttributeSet::On_RepArmour(const FGameplayAttributeData& OldArmour)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, Armour, OldArmour)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetAttackSpeedAttribute(), OldArmour, Armour);
	}
}

void UCHAttributeSet::On_RepAttackRange(const FGameplayAttributeData& OldAttackRange)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, AttackRange, OldAttackRange)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetAttackSpeedAttribute(), OldAttackRange, AttackRange);
	}
}

void UCHAttributeSet::On_RepGold(const FGameplayAttributeData& OldGold)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, Gold, OldGold)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetGoldAttribute(), OldGold, Gold);
	}
}
