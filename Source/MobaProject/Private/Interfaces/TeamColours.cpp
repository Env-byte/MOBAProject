// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/TeamColours.h"
#include "Framework/AllMid/PSAllMid.h"
DEFINE_LOG_CATEGORY(LogTeamColours);


FVector ITeamColours::GetColour(ETeam ClientTeam)
{
	if (ClientTeam == ETeam::NeutralTeam)
	{
		return FVector(0.f, 0.f, 0.f);
	}

	const ETeam ObjTeam = GetTeam();
	if (ClientTeam == ETeam::RedTeam)
	{
		if (ObjTeam == ETeam::RedTeam)
		{
			//show allies as blue
			return FVector(0.f, 0.f, 255.f);
		}
		if (ObjTeam == ETeam::BlueTeam)
		{
			//show enemies as red
			return FVector(255.f, 0.f, 0.f);
		}
	}
	else if (ClientTeam == ETeam::BlueTeam)
	{
		if (ObjTeam == ETeam::RedTeam)
		{
			//show enemies as red
			return FVector(255.f, 0.f, 0.f);
		}
		if (ObjTeam == ETeam::BlueTeam)
		{
			//show allies as blue
			return FVector(0.f, 0.f, 255.f);
		}
	}
	return FVector(0.f, 0.f, 0.f);
}

ETeam ITeamColours::GetTeam()
{
	return ETeam::NeutralTeam;
}

ETeam ITeamColours::GetClientTeam(UWorld* World)
{
	const APlayerController* PC = GEngine->GetFirstLocalPlayerController(World);
	if (!IsValid(PC)) { return ETeam::NeutralTeam; }
	const APSAllMid* PS = PC->GetPlayerState<APSAllMid>();
	if (!IsValid(PS)) { return ETeam::NeutralTeam; }
	return PS->Team;
}
