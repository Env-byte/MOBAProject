// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Nexus.h"

#include "Characters/CHAttributeSet.h"
#include "Characters/CHGameplayAbility.h"
#include "Components/CharacterNamePlate.h"
#include "Framework/AllMid/GMAllMid.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/Components/WNamePlate.h"
DEFINE_LOG_CATEGORY(LogCHNexus);

// Sets default values
ANexus::ANexus()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	AbilitySystemComponent = CreateDefaultSubobject<UCHAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	NexusMesh = CreateDefaultSubobject<UStaticMeshComponent>("NexusMesh");
	SetRootComponent(NexusMesh);

	NamePlateComponent = CreateDefaultSubobject<UCharacterNamePlate>("NamePlateComponent");
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
	NamePlateComponent->AttachToComponent(GetRootComponent(), AttachmentRules);

	Attributes = CreateDefaultSubobject<UCHAttributeSet>("Attributes");
}

FActorHelper ANexus::GetActorInfo()
{
	return FActorHelper{Team, this};
}

ETeam ANexus::GetTeam()
{
	return Team;
}

void ANexus::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANexus, Team);
}

FName ANexus::GetEntityName()
{
	return FName("Nexus");
}

int32 ANexus::GetCharacterLevel() const
{
	return 1;
}

void ANexus::InitializeAttributes()
{
	if (!HasAuthority()) return;

	if (!AbilitySystemComponent)
	{
		UE_LOG(LogCHNexus, Error, TEXT("AbilitySystemComponent not valid"));
		return;
	}
	if (!DefaultAttributeEffect)
	{
		UE_LOG(LogCHNexus, Error, TEXT("DefaultAttributeEffect not valid"));
		return;
	}

	const FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		DefaultAttributeEffect, GetCharacterLevel(), ContextHandle);

	if (SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void ANexus::InitializeOwningActor()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}


void ANexus::HandleDamage(float Damage, const FHitResult HitInfo, const FGameplayTagContainer& DamageTags,
                          ACHBase* InstigatorCharacter, AActor* DamageCauser)
{
}

void ANexus::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	if (!HasAuthority()) return;
	BP_OnHealthChanged(DeltaValue, EventTags);
	if (Attributes->GetHealth() == 0.f)
	{
		AGameModeBase* AuthGameMode = GetWorld()->GetAuthGameMode();
		if (IsValid(AuthGameMode))
		{
			AGMAllMid* GameMode = Cast<AGMAllMid>(AuthGameMode);
			if (IsValid(GameMode))
			{
				if (Team == ETeam::BlueTeam)
				{
					GameMode->NexusDestroyed(ETeam::RedTeam);
				}
				else
				{
					GameMode->NexusDestroyed(ETeam::BlueTeam);
				}
			}
		}
		Destroy();
	}
}

void ANexus::OnRep_Attribute(const FGameplayAttribute& Attribute, const FGameplayAttributeData& OldValue,
                             const FGameplayAttributeData& NewValue)
{
	UWNamePlate* NamePlate = NamePlateComponent->GetNamePlateWidget();
	if (!NamePlate) { return; }

	if (Attribute == Attributes->GetMaxHealthAttribute() || Attribute == Attributes->GetHealthAttribute())
	{
		NamePlate->SetHealthPercentage(Attributes->GetHealthPercent());
	}
}

UAbilitySystemComponent* ANexus::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ANexus::SetupNamePlateWidget()
{
	if (NamePlateComponent)
	{
		UWNamePlate* NamePlate = NamePlateComponent->GetNamePlateWidget();
		if (NamePlate)
		{
			NamePlate->HideLevel();
			NamePlate->HideMana();
			NamePlate->SetHealthPercentage(Attributes->GetHealthPercent());
			NamePlate->SetName(GetEntityName());
		}
	}
}

// Called when the game starts or when spawned
void ANexus::BeginPlay()
{
	Super::BeginPlay();
	InitializeOwningActor();
	SetupNamePlateWidget();
	Execute_SetColour(this, GetColour(GetClientTeam(GetWorld())));
}
