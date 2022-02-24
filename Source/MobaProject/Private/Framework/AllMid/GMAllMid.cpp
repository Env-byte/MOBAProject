// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/AllMid/GMAllMid.h"

#include "Framework/AllMid/PCAllMid.h"
#include "Framework/AllMid/PSAllMid.h"
DEFINE_LOG_CATEGORY(LogArenaGM);

AGMAllMid::AGMAllMid()
{
	ExpectedPlayerCount = 2;
	ReadyPlayers = 0;
	Countdown = 5.f;
}

void AGMAllMid::CountdownFinished()
{
	UE_LOG(LogArenaGM, Display, TEXT("CountdownFinished"));
	for (int32 i = 0; i < PlayerControllers.Num(); i++)
	{
		if (PlayerControllers[i] != nullptr)
		{
			PlayerControllers[i]->Client_OnGameStarted();
		}
	}
}

void AGMAllMid::StartCountdown()
{
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AGMAllMid::CountdownFinished, Countdown, false);
	for (int32 i = 0; i < PlayerControllers.Num(); i++)
	{
		if (PlayerControllers[i] != nullptr)
		{
			PlayerControllers[i]->Client_ShowNotification(FText::FromString("Game Starting"));
			PlayerControllers[i]->Client_StartGameCountdown(Countdown);
		}
	}
}

void AGMAllMid::HandlePlayerJoin(APCAllMid* PlayerController)
{
	PlayerControllers.Push(PlayerController);
	UE_LOG(LogArenaGM, Display, TEXT("ClientController: %d"), PlayerControllers.Num());

	APSAllMid* PS = PlayerController->GetPlayerState<APSAllMid>();
	if (PlayerControllers.Num() == 1)
	{
		PS->Team = ETeam::BlueTeam;
	}
	else
	{
		PS->Team = ETeam::RedTeam;
	}

	//start match if it has not already started
	if (!HasMatchStarted())
	{
		StartMatch();
	}
}

void AGMAllMid::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	APCAllMid* ClientController = Cast<APCAllMid>(NewPlayer);
	if (ClientController != nullptr)
	{
		HandlePlayerJoin(ClientController);
	}
}

void AGMAllMid::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);
	APCAllMid* ClientController = Cast<APCAllMid>(NewController);
	if (ClientController != nullptr)
	{
		HandlePlayerJoin(ClientController);
	}
}

void AGMAllMid::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void AGMAllMid::PlayerControllerReady(const APCAllMid* PlayerController)
{
	ReadyPlayers++;
	if (ReadyPlayers == ExpectedPlayerCount)
	{
		UE_LOG(LogArenaGM, Display, TEXT("All PlayerControllers Ready. Starting countdown"));
		//start match if it has not already started
		StartCountdown();
	}
}
