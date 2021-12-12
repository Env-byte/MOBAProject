// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/AllMid/PCAllMid.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Framework/AllMid/HUDAllMid.h"

APCAllMid::APCAllMid()
{
	bShowMouseCursor = true;
	bMoveToMouseCursor = false;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void APCAllMid::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void APCAllMid::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &APCAllMid::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &APCAllMid::OnSetDestinationReleased);

	InputComponent->BindAction("ToggleScoreboard", IE_Pressed, this, &APCAllMid::OnScoreboardPressed);
	InputComponent->BindAction("ToggleScoreboard", IE_Released, this, &APCAllMid::OnScoreboardReleased);
}

void APCAllMid::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(Hit.ImpactPoint);
	}
}


void APCAllMid::SetNewMoveDestination(const FVector DestLocation)
{
	if (!HasAuthority())
	{
		Server_SetNewMoveDestination(DestLocation);
	}
	const APawn* MyPawn = GetPawn();
	if (MyPawn)
	{
		// We need to issue move command only if far enough in order for walk animation to play correctly
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
	}
}

void APCAllMid::Server_SetNewMoveDestination_Implementation(const FVector& DestLocation)
{
	SetNewMoveDestination(DestLocation);
}


void APCAllMid::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void APCAllMid::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void APCAllMid::OnScoreboardPressed()
{
	AHUDAllMid* Hud = GetHUD<AHUDAllMid>();
	if (Hud)
	{
		Hud->ShowScoreboard();
	}
}

void APCAllMid::OnScoreboardReleased()
{
	AHUDAllMid* Hud = GetHUD<AHUDAllMid>();
	if (Hud)
	{
		Hud->HideScoreboard();
	}
}
