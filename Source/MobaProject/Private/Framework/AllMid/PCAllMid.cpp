// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/AllMid/PCAllMid.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Characters/Playable/CHPlayable.h"
#include "Components/InventoryComponent.h"
#include "Framework/AllMid/HUDAllMid.h"
#include "Framework/AllMid/PSAllMid.h"
#include "GameFramework/SpringArmComponent.h"
#include "Widgets/AllMid/WPlayerHud.h"



APCAllMid::APCAllMid()
{
	bShowMouseCursor = true;
	bMoveToMouseCursor = false;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void APCAllMid::PlayerStateReady(APSAllMid* PS)
{
	PS->InventoryComponent->OnInventoryUpdated.AddDynamic(this, &APCAllMid::OnPlayerInventoryUpdated);
}

void APCAllMid::OnPlayerInventoryUpdated(const TArray<UBaseItem*>& Items)
{
	UE_LOG(LogTemp, Display, TEXT("OnPlayerInventoryUpdated"))
	AHUDAllMid* HUD = GetHUD<AHUDAllMid>();
	if (HUD)
	{
		HUD->GetPlayerHudWidget()->BP_SetInventory(Items);
	}
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

	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &APCAllMid::OnZoomInPressed);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &APCAllMid::OnZoomOutPressed);
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

void APCAllMid::OnZoomInPressed()
{
	UE_LOG(LogTemp, Display, TEXT("OnZoomInPressed"))
	const ACHPlayable* PlayerCharacter = GetPawn<ACHPlayable>();
	if (!PlayerCharacter) return;

	USpringArmComponent* SpringArmComponent = PlayerCharacter->GetCameraBoom();
	if (!SpringArmComponent) return;

	float CurrentLength = SpringArmComponent->TargetArmLength;
	CurrentLength = CurrentLength + 50;
	UE_LOG(LogTemp, Display, TEXT("CurrentLength: %f"), CurrentLength)

	SpringArmComponent->TargetArmLength = FMath::Clamp(CurrentLength, MinZoom, MaxZoom);
}

void APCAllMid::OnZoomOutPressed()
{
	UE_LOG(LogTemp, Display, TEXT("OnZoomOutPressed"))
	const ACHPlayable* PlayerCharacter = GetPawn<ACHPlayable>();
	if (!PlayerCharacter) return;

	USpringArmComponent* SpringArmComponent = PlayerCharacter->GetCameraBoom();
	if (!SpringArmComponent) return;

	float CurrentLength = SpringArmComponent->TargetArmLength;
	CurrentLength = CurrentLength - 50;
	UE_LOG(LogTemp, Display, TEXT("CurrentLength: %f"), CurrentLength)
	SpringArmComponent->TargetArmLength = FMath::Clamp(CurrentLength, MinZoom, MaxZoom);
}
