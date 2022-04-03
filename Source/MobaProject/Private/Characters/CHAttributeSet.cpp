// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CHAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Characters/CHBase.h"
#include "Characters/Playable/CHPlayable.h"
#include "Framework/AllMid/HUDAllMid.h"
#include "Framework/AllMid/PCAllMid.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/AllMid/WPlayerHud.h"
#include "Widgets/AllMid/WSelectedTarget.h"
class AHUDAllMid;
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
	DOREPLIFETIME_CONDITION_NOTIFY(UCHAttributeSet, HealthRegenRate, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UCHAttributeSet, ManaRegenRate, COND_None, REPNOTIFY_Always)
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

	ICanTakeDamage* Target{nullptr};
	ACHPlayable* SourcePlayer{nullptr};
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		if (TargetActor->Implements<UCanTakeDamage>())
		{
			Target = Cast<ICanTakeDamage>(TargetActor);
		}
	}

	if (IsValid(Source->GetAvatarActor()))
	{
		SourcePlayer = Cast<ACHPlayable>(Source->GetAvatarActor());
	}

	UE_LOG(LogCHAttributeSet, Display, TEXT("ICanTakeDamage* Target : %p"), Target)

	//notify character that health has changed. Also prevent health going below 0 or exceeding max
	//make sure to set MaxHealth Before Health when init attributes
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		if (Target)
		{
			Target->HandleHealthChanged(DeltaValue, SourceTags, SourcePlayer);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		//prevent mana going below 0 or exceeding max
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
}

void UCHAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,
                                                  const FGameplayAttributeData& MaxAttribute, float NewMaxValue,
                                                  const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilitySystemComponent = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilitySystemComponent)
	{
		const float Difference = NewMaxValue - CurrentMaxValue;
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		const float NewDelta = FMath::Clamp(Difference + CurrentValue, 0.f, NewMaxValue);
		UE_LOG(LogTemp, Display, TEXT("Adjust Attribute %s Adding %f. New Value %f"),
		       *AffectedAttributeProperty.GetName(), Difference, NewDelta)

		//AbilitySystemComponent->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
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

void UCHAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, Health, OldHealth)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetHealthAttribute(), OldHealth, Health);
	}
	NotifyPlayerController();
}

void UCHAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, MaxHealth, OldHealth)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetMaxHealthAttribute(), OldHealth, MaxHealth);
	}
	NotifyPlayerController();
}

void UCHAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, Mana, OldMana)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetManaAttribute(), OldMana, Mana);
	}
	NotifyPlayerController();
}

void UCHAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, MaxMana, OldMaxMana)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetMaxManaAttribute(), OldMaxMana, MaxMana);
	}
	NotifyPlayerController();
}

void UCHAttributeSet::OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, AttackDamage, OldAttackDamage)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetAttackDamageAttribute(), OldAttackDamage, AttackDamage);
	}
	NotifyPlayerController();
}

void UCHAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, MoveSpeed, OldMoveSpeed)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetMoveSpeedAttribute(), OldMoveSpeed, MoveSpeed);
	}
	NotifyPlayerController();
}

void UCHAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, AttackSpeed, OldAttackSpeed)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetAttackSpeedAttribute(), OldAttackSpeed, AttackSpeed);
	}
	NotifyPlayerController();
}

void UCHAttributeSet::OnRep_Armour(const FGameplayAttributeData& OldArmour)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, Armour, OldArmour)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetAttackSpeedAttribute(), OldArmour, Armour);
	}
	NotifyPlayerController();
}

void UCHAttributeSet::OnRep_AttackRange(const FGameplayAttributeData& OldAttackRange)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, AttackRange, OldAttackRange)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetAttackSpeedAttribute(), OldAttackRange, AttackRange);
	}
	NotifyPlayerController();
}

void UCHAttributeSet::OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, HealthRegenRate, OldHealthRegenRate)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetHealthRegenRateAttribute(), OldHealthRegenRate, HealthRegenRate);
	}
	NotifyPlayerController();
}

void UCHAttributeSet::OnRep_ManaRegenRate(const FGameplayAttributeData& OldManaRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHAttributeSet, ManaRegenRate, OldManaRegenRate)
	ACHBase* BaseCharacter = GetOwningActor();
	if (BaseCharacter)
	{
		BaseCharacter->OnRep_Attribute(GetManaRegenRateAttribute(), OldManaRegenRate, ManaRegenRate);
	}
	NotifyPlayerController();
}

void UCHAttributeSet::NotifyPlayerController() const
{
	const UWorld* World = GetWorld();
	if (IsValid(World))
	{
		APlayerController* PC = GEngine->GetFirstLocalPlayerController(World);
		if (IsValid(PC))
		{
			const APCAllMid* APC = Cast<APCAllMid>(PC);
			APC->UpdateTargetingWidget(this);
		}
	}
}
