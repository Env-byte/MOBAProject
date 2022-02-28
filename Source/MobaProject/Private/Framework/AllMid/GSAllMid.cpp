// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/AllMid/GSAllMid.h"

#include "Framework/AllMid/HUDAllMid.h"
#include "Framework/AllMid/PSAllMid.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/AllMid/WGameOverview.h"
#include "Widgets/AllMid/WPlayerHud.h"
DEFINE_LOG_CATEGORY(LogGSAllMid);

void AGSAllMid::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGSAllMid, BlueTowersDestroyed)
	DOREPLIFETIME(AGSAllMid, RedTowersDestroyed)
}

void AGSAllMid::OnRep_TowersDestroyed()
{
	for (APlayerState* Player : PlayerArray)
	{
		if (!Player)
		{
			UE_LOG(LogGSAllMid, Warning, TEXT("APlayerState Not Valid"))
			return;
		}
		const APSAllMid* PS = Cast<APSAllMid>(Player);
		if (!IsValid(PS))
		{
			UE_LOG(LogGSAllMid, Warning, TEXT("APSAllMid Not Valid"))
			return;
		}
		if (IsValid(PS->PlayerControllerRef) && PS->PlayerControllerRef->IsLocalController())
		{
			const AHUDAllMid* Hud = PS->PlayerControllerRef->GetHUD<AHUDAllMid>();
			if (IsValid(Hud))
			{
				const UWPlayerHud* HudWidget = Hud->GetPlayerHudWidget();
				if (IsValid(HudWidget))
				{
					HudWidget->GetGameOverview()->UpdateTowersDestroyed(RedTowersDestroyed, BlueTowersDestroyed);
				}
			}
		}
	}
}

void AGSAllMid::IncrementDestroyedTower(const ETeam Team)
{
	if (!HasAuthority()) { return; }
	if (Team == ETeam::BlueTeam)
	{
		BlueTowersDestroyed++;
	}
	else if (Team == ETeam::RedTeam)
	{
		RedTowersDestroyed++;
	}
}

void AGSAllMid::BeginPlay()
{
	Super::BeginPlay();
}
