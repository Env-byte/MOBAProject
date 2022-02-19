// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GMAllMid.generated.h"
DECLARE_LOG_CATEGORY_EXTERN(LogArenaGM, Log, All);

/**
 * 
 */
UCLASS(Abstract)
class MOBAPROJECT_API AGMAllMid : public AGameMode
{
	GENERATED_BODY()

public:
	AGMAllMid();
protected:
	////////// Players Joining //////////
	/**
	 * The game countdown
	 */
	float Countdown;

	/** All active player controllers */
	UPROPERTY()
	TArray<class APCAllMid*> PlayerControllers;

	/** Number of players that are expected*/
	UPROPERTY(EditDefaultsOnly)
	int32 ExpectedPlayerCount;

	/**
	 * Number of players Ready
	 */
	UPROPERTY()
	int32 ReadyPlayers;

	/** executed x second after StartMatch() was called. Releases player control and remove on screen text */
	void CountdownFinished();

	/**
	 * Instruct all clients to start game countdown
	 */
	void StartCountdown();

	/**
	 * Handles a player joining the server from either server travel or direct login
	 */
	void HandlePlayerJoin(APCAllMid* PlayerController);
public:
	/** if joining straight into the server */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/** Called when the player controller swaps after server travel */
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;

	/** Handle player leaving by removing from array of all players */
	virtual void Logout(AController* Exiting) override;

	/** When clientside is finished setting up, this function is called from the playerController */
	void PlayerControllerReady(const APCAllMid* PlayerController);
	////////// Players Joining //////////
};
