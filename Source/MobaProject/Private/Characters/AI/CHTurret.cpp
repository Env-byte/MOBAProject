// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/CHTurret.h"

#include "Components/CharacterNamePlate.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/Components/WNamePlate.h"
DEFINE_LOG_CATEGORY(LogCHTurret);

// Sets default values
ACHTurret::ACHTurret()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	AbilitySystemComponent = CreateDefaultSubobject<UCHAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>("TowerMesh");
	SetRootComponent(TowerMesh);

	NamePlateComponent = CreateDefaultSubobject<UCharacterNamePlate>("NamePlateComponent");
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
	NamePlateComponent->AttachToComponent(GetRootComponent(), AttachmentRules);

	Attributes = CreateDefaultSubobject<UCHAttributeSet>("Attributes");
}

FActorHelper ACHTurret::GetActorInfo()
{
	return FActorHelper{Team, this};
}

ETeam ACHTurret::GetTeam()
{
	return Team;
}

void ACHTurret::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACHTurret, Team);
}

FName ACHTurret::GetEntityName()
{
	return FName("Turret");
}

int32 ACHTurret::GetCharacterLevel() const
{
	return 1;
}

void ACHTurret::InitializeAttributes()
{
	if (!HasAuthority()) return;

	if (!AbilitySystemComponent)
	{
		UE_LOG(LogCHTurret, Error, TEXT("AbilitySystemComponent not valid"));
		return;
	}
	if (!DefaultAttributeEffect)
	{
		UE_LOG(LogCHTurret, Error, TEXT("DefaultAttributeEffect not valid"));
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

void ACHTurret::InitializeOwningActor()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void ACHTurret::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (AbilitySystemComponent)
	{
		InitializeOwningActor();
		InitializeAttributes();
	}
}

void ACHTurret::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	if (AbilitySystemComponent && InputComponent)
	{
		InitializeOwningActor();
	}
}

void ACHTurret::HandleDamage(float Damage, const FHitResult HitInfo, const FGameplayTagContainer& DamageTags,
                             ACHBase* InstigatorCharacter, AActor* DamageCauser)
{
	BP_OnDamaged(Damage, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

void ACHTurret::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags, ACHPlayable* SourcePlayer)
{
	if (!HasAuthority()) return;
	BP_OnHealthChanged(DeltaValue, EventTags);
	if (Attributes->GetHealth() == 0.f)
	{
		Destroy();
	}
}

void ACHTurret::OnRep_Attribute(const FGameplayAttribute& Attribute, const FGameplayAttributeData& OldValue,
                                const FGameplayAttributeData& NewValue)
{
	UWNamePlate* NamePlate = NamePlateComponent->GetNamePlateWidget();
	if (!NamePlate) { return; }

	if (Attribute == Attributes->GetMaxHealthAttribute() || Attribute == Attributes->GetHealthAttribute())
	{
		NamePlate->SetHealthPercentage(Attributes->GetHealthPercent());
	}
	else if (Attribute == Attributes->GetManaAttribute() || Attribute == Attributes->GetMaxManaAttribute())
	{
		NamePlate->SetManaPercentage(Attributes->GetManaPercent());
	}
}

UAbilitySystemComponent* ACHTurret::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACHTurret::SetupNamePlateWidget()
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
void ACHTurret::BeginPlay()
{
	Super::BeginPlay();
	SetupNamePlateWidget();
	Execute_SetColour(this, GetColour(GetClientTeam(GetWorld())));
}

// Called to bind functionality to input
void ACHTurret::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
