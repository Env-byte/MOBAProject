// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/AllMid/PSAllMid.h"

#include "Components/InventoryComponent.h"
#include "Framework/AllMid/HUDAllMid.h"
#include "Framework/AllMid/PCAllMid.h"
#include "Framework/AllMid/PSAbilitySystemComponent.h"
#include "Framework/AllMid/PSAttributeSet.h"
#include "Widgets/AllMid/WPlayerHud.h"
DEFINE_LOG_CATEGORY(LogPSAllMid);

APSAllMid::APSAllMid()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	InventoryComponent->SetCapacity(6);

	AbilitySystemComponent = CreateDefaultSubobject<UPSAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	Attributes = CreateDefaultSubobject<UPSAttributeSet>("Attributes");
}

void APSAllMid::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = GetOwner<APCAllMid>();
	if (PlayerControllerRef)
	{
		if (PlayerControllerRef->IsLocalController())
		{
			PlayerControllerRef->PlayerStateReady(this);
		}
		if (PlayerControllerRef->HasAuthority())
		{
			InitializeAttributes();
		}
	}
}

void APSAllMid::InitializeAttributes()
{
	const FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 0, ContextHandle);
	UE_LOG(LogPSAllMid, Display, TEXT("%s InitializeAttributes"), *this->GetName())

	if (SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}

	const FGameplayEffectContextHandle GoldContextHandle = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle GoldSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GoldPassiveEffect, 0, GoldContextHandle);
	if (SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GoldSpecHandle.Data.Get());
	}
}

UAbilitySystemComponent* APSAllMid::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APSAllMid::OnRep_Attribute(const FGameplayAttribute& Attribute, const FGameplayAttributeData& OldValue, const FGameplayAttributeData& NewValue)
{
	if (!PlayerControllerRef) { return; }
	if (!PlayerControllerRef->IsLocalController()) { return; }
	AHUDAllMid* HUD = PlayerControllerRef->GetHUD<AHUDAllMid>();
	if (!HUD) { return; }
	UWPlayerHud* PlayerHudWidget = HUD->GetPlayerHudWidget();
	if (!PlayerHudWidget) { return; }
	
	if (Attribute == Attributes->GetGoldAttribute())
	{
		PlayerHudWidget->BP_SetGold(NewValue.GetCurrentValue());
	}
}
