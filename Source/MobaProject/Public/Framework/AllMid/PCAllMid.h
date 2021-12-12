// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PCAllMid.generated.h"

/**
 * 
 */
UCLASS()
class MOBAPROJECT_API APCAllMid : public APlayerController
{
	GENERATED_BODY()

	APCAllMid();
protected:
	virtual void SetupInputComponent() override;

	////////// PLAYER MOVEMENT //////////
	bool bMoveToMouseCursor;
	virtual void PlayerTick(float DeltaTime) override;
	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);
	UFUNCTION(Server, Unreliable)
	void Server_SetNewMoveDestination(const FVector& DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
	////////// PLAYER MOVEMENT //////////

	////////// Scoreboard //////////
	void OnScoreboardPressed();
	void OnScoreboardReleased();
	////////// Scoreboard //////////
};
