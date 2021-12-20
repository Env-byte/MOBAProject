// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Playable/CHPlayable.h"

#include "Camera/CameraComponent.h"
#include "Characters/CHAbilitySystemComponent.h"
#include "Characters/CHAttributeSet.h"
#include "Characters/CHGameplayAbility.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Framework/AllMid/HUDAllMid.h"
#include "Framework/AllMid/PCAllMid.h"
#include "Framework/AllMid/PSAllMid.h"
#include "Framework/AllMid/PSAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/ConsumableItem.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/AllMid/WPlayerHud.h"

DEFINE_LOG_CATEGORY(LogCHPlayable);

ACHPlayable::ACHPlayable()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	NextPrimaryAttackIndex = 0;
	bIsAttacking = false;
	bAbilitiesInitialized = false;
}

void ACHPlayable::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!HasAuthority())
	{
		UpdateCursorDecal();
	}

	if (GetWorld()->TimeSince(LastHealthTick) > HealthTickFrequency)
	{
		ApplyPassiveRegen();
	}
}

void ACHPlayable::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	APCAllMid* PC = GetController<APCAllMid>();
	if (!PC) { return; }

	AHUDAllMid* HUD = PC->GetHUD<AHUDAllMid>();
	if (!HUD) { return; }

	UWPlayerHud* PlayerHudWidget = HUD->GetPlayerHudWidget();
	if (!PlayerHudWidget) { return; }
	PlayerHudWidget->BP_PlayerSpawned(this);
}

void ACHPlayable::UpdateCursorDecal() const
{
	if (CursorToWorld != nullptr)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			const FVector CursorFV = TraceHitResult.ImpactNormal;
			const FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}

void ACHPlayable::BeginPlay()
{
	Super::BeginPlay();
}

void ACHPlayable::ApplyPassiveRegen()
{
	const float CurrentMana = Attributes->GetMana();
	const float CurrentHealth = Attributes->GetHealth();
	const float MaxMana = Attributes->GetMaxMana();
	const float MaxHealth = Attributes->GetMaxHealth();

	if (CurrentMana != MaxMana)
	{
		Attributes->SetMana(FMath::Clamp(CurrentMana + Attributes->GetManaRegenRate(), 0.f, MaxMana));
	}
	if (CurrentHealth != MaxHealth)
	{
		Attributes->SetHealth(FMath::Clamp(CurrentHealth + Attributes->GetHealthRegenRate(), 0.f, MaxHealth));
	}
	LastHealthTick = GetWorld()->GetTimeSeconds();
}

void ACHPlayable::GiveAbilities()
{
	if (HasAuthority() && !bAbilitiesInitialized && AbilitySystemComponent)
	{
		const int32 Level = GetCharacterLevel();
		if (PrimaryAttack)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(PrimaryAttack, Level, static_cast<int32>(PrimaryAttack.GetDefaultObject()->AbilityInputID), this));
		}
		if (Ability1)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(Ability1, Level, static_cast<int32>(Ability1.GetDefaultObject()->AbilityInputID), this));
		}
		if (Ability2)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(Ability2, Level, static_cast<int32>(Ability2.GetDefaultObject()->AbilityInputID), this));
		}
		if (Ability3)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(Ability3, Level, static_cast<int32>(Ability3.GetDefaultObject()->AbilityInputID), this));
		}
		bAbilitiesInitialized = true;
	}
}

void ACHPlayable::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	GiveAbilities();
}

void ACHPlayable::OnRep_Attribute(const FGameplayAttribute& Attribute, const FGameplayAttributeData& OldValue, const FGameplayAttributeData& NewValue)
{
	Super::OnRep_Attribute(Attribute, OldValue, NewValue);
	if (!IsLocallyControlled()) { return; }

	APCAllMid* PC = GetController<APCAllMid>();
	if (!PC) { return; }
	if (!PC->IsLocalController()) { return; }
	AHUDAllMid* HUD = PC->GetHUD<AHUDAllMid>();
	if (!HUD) { return; }

	UWPlayerHud* PlayerHudWidget = HUD->GetPlayerHudWidget();
	if (!PlayerHudWidget) { return; }

	//UE_LOG(LogCHPlayable, Display, TEXT("%s OnRep_Attribute %s From %f To %f"), *this->GetName(), *Attribute.GetName(), OldValue.GetCurrentValue(), NewValue.GetCurrentValue())

	// if attribute updated is not health or mana then update stats
	if (Attribute == Attributes->GetMaxHealthAttribute() || Attribute == Attributes->GetHealthAttribute())
	{
		PlayerHudWidget->BP_SetHealth(Attributes->GetHealth(), Attributes->GetMaxHealth());
	}
	else if (Attribute == Attributes->GetManaAttribute() || Attribute == Attributes->GetMaxManaAttribute())
	{
		PlayerHudWidget->BP_SetMana(Attributes->GetMana(), Attributes->GetMaxMana());
	}
	else
	{
		PlayerHudWidget->BP_SetCharacterStats(Attributes);
	}
}

void ACHPlayable::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& GameplayTags)
{
	Super::HandleHealthChanged(DeltaValue, GameplayTags);
}

int32 ACHPlayable::GetCharacterLevel() const
{
	APSAllMid* ThisPlayerState = GetPlayerState<APSAllMid>();
	if (!ThisPlayerState) { return 0; }
	return static_cast<int32>(ThisPlayerState->GetAttributeSet()->GetCharacterLevel());
}
