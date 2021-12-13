// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Items/BaseItem.h"
#include "PCAllMid.generated.h"

class APSAllMid;
class UInventoryComponent;
/**
 * 
 */
UCLASS()
class MOBAPROJECT_API APCAllMid : public APlayerController
{
	GENERATED_BODY()
public:
	APCAllMid();
	
	/**
	 * Called straight after player state begin play. Only called for local player controllers
	 */
	void PlayerStateReady(APSAllMid* PS);
protected:
	UFUNCTION()
	void OnPlayerInventoryUpdated(const TArray<UBaseItem*>& Items);

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

	////////// Camera Zoom //////////
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Zoom")
	float MinZoom = 700.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Zoom")
	float MaxZoom = 1400.f;
	void OnZoomInPressed();
	void OnZoomOutPressed();
	////////// Camera Zoom //////////
};
