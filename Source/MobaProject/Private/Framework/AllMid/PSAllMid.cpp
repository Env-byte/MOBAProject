// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/AllMid/PSAllMid.h"
#include "Characters/AI/CHTurret.h"
#include "Characters/Playable/CHPlayable.h"
#include "Components/InventoryComponent.h"
#include "Framework/AllMid/HUDAllMid.h"
#include "Framework/AllMid/PCAllMid.h"
#include "Framework/AllMid/PSAbilitySystemComponent.h"
#include "Framework/AllMid/PSAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/AllMid/WGameOverview.h"
#include "Widgets/AllMid/WGameScoreboard.h"
#include "Widgets/AllMid/WGameScoreboardItem.h"
#include "Widgets/AllMid/WPlayerHud.h"
DEFINE_LOG_CATEGORY(LogPSAllMid);

APSAllMid::APSAllMid()
{
	PlayerControllerRef = nullptr;
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

	InventoryComponent->OnInventoryUpdated.AddDynamic(this, &APSAllMid::OnPlayerInventoryUpdated);

	PlayerControllerRef = GetOwner<APCAllMid>();
	if (PlayerControllerRef)
	{
		if (PlayerControllerRef->HasAuthority())
		{
			InitializeAttributes();
		}
	}
	if (!HasAuthority())
	{
		CreateScoreboardItem();
	}
}

void APSAllMid::OnPlayerInventoryUpdated(const TArray<UBaseItem*>& Items)
{
	if (HasAuthority())
	{
		UE_LOG(LogPSAllMid, Display, TEXT("OnPlayerInventoryUpdated Has Auth"))
		return;
	}

	if (ScoreboardItem)
	{
		ScoreboardItem->UpdateWidget();
	}

	if (IsValid(PlayerControllerRef) && PlayerControllerRef->IsLocalController())
	{
		// is local player, this means its friendly 
		AHUDAllMid* HUD = PlayerControllerRef->GetHUD<AHUDAllMid>();
		if (HUD)
		{
			HUD->GetPlayerHudWidget()->SetInventory(Items);
		}
	}
}

void APSAllMid::InitializeAttributes()
{
	const FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		DefaultAttributeEffect, 0, ContextHandle);
	UE_LOG(LogPSAllMid, Display, TEXT("%s InitializeAttributes"), *this->GetName())

	if (SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}

	const FGameplayEffectContextHandle GoldContextHandle = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle GoldSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		GoldPassiveEffect, 0, GoldContextHandle);
	if (SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GoldSpecHandle.Data.Get());
	}
}

void APSAllMid::Client_UseTeamColours_Implementation(ETeam ThisTeam)
{
	UseTeamColours(ThisTeam);
}

void APSAllMid::CreateScoreboardItem()
{
	APlayerController* PC;
	ETeam LocalFriendlyTeam;
	if (IsValid(PlayerControllerRef) && PlayerControllerRef->IsLocalController())
	{
		//this is the local player state
		LocalFriendlyTeam = Team;
		PC = Cast<APlayerController>(PlayerControllerRef);
	}
	else
	{
		//this is not the local player state
		PC = GEngine->GetFirstLocalPlayerController(GetWorld());
		LocalFriendlyTeam = PC->GetPlayerState<APSAllMid>()->Team;
	}
	if (!IsValid(PC))
	{
		UE_LOG(LogPSAllMid, Display, TEXT("PC Is not valid"))
		return;
	}
	ScoreboardItem = CreateWidget<UWGameScoreboardItem>
	(
		PC,
		ScoreboardItemClass,
		FName(FString::Printf(TEXT("%sScoreboardItem"), *GetName()))
	);

	ScoreboardItem->SetPlayerState(this);

	const AHUDAllMid* HUD = PC->GetHUD<AHUDAllMid>();
	if (LocalFriendlyTeam == Team)
	{
		//add to friendly team
		HUD->GameScoreboard->AddFriendlyItem(this, ScoreboardItem);
	}
	else
	{
		//add to enemy team
		HUD->GameScoreboard->AddEnemyItem(this, ScoreboardItem);
	}
}

void APSAllMid::UseTeamColours(ETeam ThisTeam)
{
	if (HasAuthority())
	{
		Client_UseTeamColours(ThisTeam);
		return;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UTeamColours::StaticClass(), FoundActors);

	UE_LOG(LogPSAllMid, Display, TEXT("Actors Found: %d"), FoundActors.Num())
	for (AActor* Actor : FoundActors)
	{
		if (!IsValid(Actor) || !Actor->Implements<UTeamColours>())
		{
			continue;
		}
		ITeamColours* TeamColour = Cast<ITeamColours>(Actor);
		if (!TeamColour) continue;
		TeamColour->Execute_SetColour(Actor, TeamColour->GetColour(ThisTeam));
	}
}

UAbilitySystemComponent* APSAllMid::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APSAllMid::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APSAllMid, Team);
}

void APSAllMid::OnRep_Attribute(const FGameplayAttribute& Attribute, const FGameplayAttributeData& OldValue,
                                const FGameplayAttributeData& NewValue)
{
	UE_LOG(LogPSAllMid, Display, TEXT("APSAllMid::OnRep_Attribute %s %f"), *Attribute.AttributeName, NewValue.GetCurrentValue())
	UWPlayerHud* PlayerHudWidget = nullptr;
	if (IsValid(PlayerControllerRef) && PlayerControllerRef->IsLocalController())
	{
		AHUDAllMid* HUD = PlayerControllerRef->GetHUD<AHUDAllMid>();
		if (!HUD) { return; }
		PlayerHudWidget = HUD->GetPlayerHudWidget();
	}

	if (Attribute == Attributes->GetGoldAttribute())
	{
		if (PlayerHudWidget)
		{
			PlayerHudWidget->BP_SetGold(NewValue.GetCurrentValue());
		}
	}
	else if (Attribute == Attributes->GetPlayersKilledAttribute())
	{
		if (PlayerHudWidget)
		{
			PlayerHudWidget->GetGameOverview()->UpdatePlayersKilled(NewValue.GetCurrentValue());
		}
		if (ScoreboardItem)
		{
			ScoreboardItem->UpdatePlayersKilled(NewValue.GetCurrentValue());
		}
	}
	else if (Attribute == Attributes->GetDeathsAttribute())
	{
		if (PlayerHudWidget)
		{
			PlayerHudWidget->GetGameOverview()->UpdatePlayerDeaths(NewValue.GetCurrentValue());
		}
		if (ScoreboardItem)
		{
			ScoreboardItem->UpdatePlayerDeaths(NewValue.GetCurrentValue());
		}
	}
	else if (Attribute == Attributes->GetMinionsKilledAttribute())
	{
		if (PlayerHudWidget)
		{
			PlayerHudWidget->GetGameOverview()->UpdateMinionsKilled(NewValue.GetCurrentValue());
		}
		if (ScoreboardItem)
		{
			ScoreboardItem->UpdateMinionsKilled(NewValue.GetCurrentValue());
		}
	}
}

void APSAllMid::OnRep_Team()
{
	//UseTeamColours(Team);
	if (!PlayerControllerRef)
	{
		UE_LOG(LogPSAllMid, Display, TEXT("PlayerControllerRef not valid"))
		return;
	}
	ACHPlayable* Pawn = PlayerControllerRef->GetPawn<ACHPlayable>();
	if (Pawn)
	{
		Pawn->Team = Team;
	}
}
