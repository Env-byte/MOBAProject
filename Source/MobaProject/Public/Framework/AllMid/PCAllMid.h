// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GMAllMid.h"
#include "GameFramework/PlayerController.h"
#include "Items/BaseItem.h"
#include "PCAllMid.generated.h"

class UConsumableItem;
class UWItemsShop;
class APSAllMid;
class UInventoryComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogPCAllMid, Log, All);

/**
 * 
 */
UCLASS()
class MOBAPROJECT_API APCAllMid : public APlayerController
{
	GENERATED_BODY()
public:
	APCAllMid();

	virtual void BeginPlay() override;

	UPROPERTY()
	AGMAllMid* GameMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ACHPlayable> PlayerCharacterClass;

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

	////////// Select //////////
	void OnSelect();
	////////// Select //////////

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
	///////////// Items //////////
	TArray<FName> ItemSlotActionNames;
	void OnUseItem(FName ActionName, int32 Index);

	/**
	 * Check if the player is in the radius of the shop
	 */
	bool IsInShop();

	/**
	 * Buys the selected item and adds it the players inventory. Also replicates to other clients
	 */
	UFUNCTION(BlueprintCallable, Category="Items")
	void BuyItem(TSubclassOf<UBaseItem> Item);

	UFUNCTION(Server, Reliable)
	void Server_BuyItem(TSubclassOf<UBaseItem> Item);

	/**
	* Sells the selected item and removes it from the players inventory. Also replicates to other clients
	*/
	UFUNCTION(BlueprintCallable, Category="Items")
	void SellItem(UBaseItem* Item);

	UFUNCTION(Server, Reliable)
	void Server_SellItem(UBaseItem* Item);

	/**
	* Consume the selected item and activating it and removing 1 of it from the players inventory. Also replicates to other clients
	*/
	UFUNCTION(BlueprintCallable, Category="Items")
	void ConsumeItem(UConsumableItem* ConsumableItem);

	UFUNCTION(Server, Reliable)
	void Server_ConsumeItem(UConsumableItem* ConsumableItem);
	////////// Items //////////
	////////// Players Joining //////////
public:
	UFUNCTION(Client, Reliable)
	void Client_OnGameStarted();

	/** Start the countdown for players  */
	UFUNCTION(Client, Reliable)
	void Client_StartGameCountdown(float StartFrom);

	/**
	 * Called when this player controller is ready to start the game, calls the game mode to let it know
	 */
	UFUNCTION(Server, Reliable)
	void Server_ReadyToStart();

	/**
	 * Runs on client, only called if ShowNotification is called on server
	 */
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_ShowNotification(const FText& Message);

	/**
	 * Notifications is handled in blueprint
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void BP_ShowNotification(const FText& Message);
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGameStarted();
	////////// Players Joining //////////

public:
	UFUNCTION(Client, Reliable)
	void Client_GameEnded(ETeam WinningTeam);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGameEnd(bool bWon);
};
