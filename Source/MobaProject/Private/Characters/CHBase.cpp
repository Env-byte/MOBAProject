// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CHBase.h"

#include "Characters/CHAbilitySystemComponent.h"
#include "Characters/CHAttributeSet.h"
#include "MobaProject/MobaProject.h"
DEFINE_LOG_CATEGORY(LogCHBase);

// Sets default values
ACHBase::ACHBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UCHAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	Attributes = CreateDefaultSubobject<UCHAttributeSet>("Attributes");
}

void ACHBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InitializeAbilityBinds();
}

int32 ACHBase::GetLevel()
{
	//by default return 1;
	return 1;
}

void ACHBase::InitializeAttributes()
{
	if (!HasAuthority()) return;
	
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogCHBase, Error, TEXT("AbilitySystemComponent not valid"));
		return;
	}
	if (!DefaultAttributeEffect)
	{
		UE_LOG(LogCHBase, Error, TEXT("DefaultAttributeEffect not valid"));
		return;
	}

	const FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, GetLevel(), ContextHandle);

	if (SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void ACHBase::InitializeOwningActor()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void ACHBase::InitializeAbilityBinds()
{
	const FGameplayAbilityInputBinds Binds(
		"Confirm",
		"Cancel",
		"EAbilityInputID",
		static_cast<int32>(EAbilityInputID::Confirm),
		static_cast<int32>(EAbilityInputID::Cancel)
	);
	AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
}

void ACHBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (AbilitySystemComponent)
	{
		InitializeOwningActor();
		InitializeAttributes();
	}
}

void ACHBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	if (AbilitySystemComponent && InputComponent)
	{
		InitializeOwningActor();
		InitializeAbilityBinds();
	}
}

void ACHBase::HandleDamage(float Damage, const FHitResult HitInfo, const FGameplayTagContainer& DamageTags, ACHBase* InstigatorCharacter, AActor* DamageCauser)
{
	BP_OnDamaged(Damage, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

void ACHBase::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	BP_OnHealthChanged(DeltaValue, EventTags);
}

UAbilitySystemComponent* ACHBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
