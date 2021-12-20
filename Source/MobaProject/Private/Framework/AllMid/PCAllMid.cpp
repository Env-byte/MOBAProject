// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/AllMid/PCAllMid.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Characters/Playable/CHPlayable.h"
#include "Components/InventoryComponent.h"
#include "Framework/AllMid/HUDAllMid.h"
#include "Framework/AllMid/PSAllMid.h"
#include "Framework/AllMid/PSAttributeSet.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/ConsumableItem.h"
#include "Widgets/AllMid/WPlayerHud.h"
#include "World/Shop.h"

APCAllMid::APCAllMid()
{
	bShowMouseCursor = true;
	bMoveToMouseCursor = false;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	ItemSlotActionNames.Add("ItemSlot1");
	ItemSlotActionNames.Add("ItemSlot2");
	ItemSlotActionNames.Add("ItemSlot3");
	ItemSlotActionNames.Add("ItemSlot4");
	ItemSlotActionNames.Add("ItemSlot5");
	ItemSlotActionNames.Add("ItemSlot6");
}

void APCAllMid::PlayerStateReady(APSAllMid* PS)
{
	PS->InventoryComponent->OnInventoryUpdated.AddDynamic(this, &APCAllMid::OnPlayerInventoryUpdated);
}

void APCAllMid::OnPlayerInventoryUpdated(const TArray<UBaseItem*>& Items)
{
	AHUDAllMid* HUD = GetHUD<AHUDAllMid>();
	if (HUD)
	{
		HUD->GetPlayerHudWidget()->SetInventory(Items);
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


	for (int32 i = 0, IL = ItemSlotActionNames.Num(); i < IL; i++)
	{
		FInputActionBinding NewActionBinding = FInputActionBinding(ItemSlotActionNames[i], IE_Pressed);
		FInputActionHandlerSignature NewDelegate;

		NewDelegate.BindLambda([this, i]()
		{
			OnUseItem(ItemSlotActionNames[i], i);
		});
		NewActionBinding.ActionDelegate = NewDelegate;
		InputComponent->AddActionBinding(NewActionBinding);
	}

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &APCAllMid::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &APCAllMid::OnSetDestinationReleased);

	InputComponent->BindAction("Select", IE_Pressed, this, &APCAllMid::OnSelect);

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

void APCAllMid::OnSelect()
{
	FHitResult HitResult;
	if (GetHitResultUnderCursorByChannel(TraceTypeQuery1, true, HitResult))
	{
		if (HitResult.Actor.IsValid())
		{
			AActor* HitActor = HitResult.Actor.Get();
			AShop* ShopActor = Cast<AShop>(HitActor);
			if (ShopActor)
			{
				GetHUD<AHUDAllMid>()->GetPlayerHudWidget()->ShowShop();
			}
		}
	}
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
	const ACHPlayable* PlayerCharacter = GetPawn<ACHPlayable>();
	if (!PlayerCharacter) return;

	USpringArmComponent* SpringArmComponent = PlayerCharacter->GetCameraBoom();
	if (!SpringArmComponent) return;

	float CurrentLength = SpringArmComponent->TargetArmLength;
	CurrentLength = CurrentLength + 50;
	SpringArmComponent->TargetArmLength = FMath::Clamp(CurrentLength, MinZoom, MaxZoom);
}

void APCAllMid::OnZoomOutPressed()
{
	const ACHPlayable* PlayerCharacter = GetPawn<ACHPlayable>();
	if (!PlayerCharacter) return;

	USpringArmComponent* SpringArmComponent = PlayerCharacter->GetCameraBoom();
	if (!SpringArmComponent) return;

	float CurrentLength = SpringArmComponent->TargetArmLength;
	CurrentLength = CurrentLength - 50;
	SpringArmComponent->TargetArmLength = FMath::Clamp(CurrentLength, MinZoom, MaxZoom);
}

void APCAllMid::OnUseItem(const FName ActionName, const int32 Index)
{
	const APSAllMid* PS = GetPlayerState<APSAllMid>();
	UE_LOG(LogCHPlayable, Display, TEXT("OnUseItem: %s %d"), *ActionName.ToString(), Index);
	TArray<UBaseItem*> Items = PS->InventoryComponent->GetItems();
	if (Items.IsValidIndex(Index))
	{
		UConsumableItem* ConsumableItem = Cast<UConsumableItem>(Items[Index]);
		if (ConsumableItem)
		{
			ConsumeItem(ConsumableItem);
		}
	}
}

bool APCAllMid::IsInShop()
{
	const ACHPlayable* Playable = GetPawn<ACHPlayable>();
	// check if player is in the shop buy area
	TArray<AActor*> FoundActors;
	Playable->GetOverlappingActors(FoundActors, AShop::StaticClass());
	if (FoundActors.Num() == 0)
	{
		return false;
	}
	return true;
}

void APCAllMid::BuyItem(const TSubclassOf<UBaseItem> Item)
{
	if (!Item) { return; }
	if (!HasAuthority())
	{
		Server_BuyItem(Item);
		return;
	}

	const APSAllMid* ThisPlayerState = GetPlayerState<APSAllMid>();
	const UBaseItem* ItemToBuy = Item->GetDefaultObject<UBaseItem>();

	//if user has more than or equal to then they can buy the item
	if (ThisPlayerState->GetAttributeSet()->GetGold() < ItemToBuy->ItemCost)
	{
		UE_LOG(LogCHPlayable, Display, TEXT("Tried to buy item failed, not enough gold. Gold: %f. Cost: %f "), ThisPlayerState->GetAttributeSet()->GetGold(), ItemToBuy->ItemCost);
		return;
	}

	ACHPlayable* Playable = GetPawn<ACHPlayable>();
	//if pawn is alive check if is in shop
	if (Playable)
	{
		if (!IsInShop())
		{
			UE_LOG(LogCHPlayable, Display, TEXT("Not in shop, cannot buy item"));
			return;
		}
	}

	const APSAllMid* PS = GetPlayerState<APSAllMid>();
	const FItemAddResult AddResult = PS->InventoryComponent->TryAddItemFromClass(Item, 1);
	if (AddResult.Item)
	{
		ThisPlayerState->GetAttributeSet()->SetGold(ThisPlayerState->GetAttributeSet()->GetGold() - ItemToBuy->ItemCost);
		AddResult.Item->OnBuy(Playable);
	}
	UE_LOG(LogCHPlayable, Display, TEXT("AddResult: %s"), *AddResult.ToString());
}

void APCAllMid::SellItem(UBaseItem* Item)
{
	if (!Item) { return; }
	if (!HasAuthority())
	{
		Server_SellItem(Item);
		return;
	}

	ACHPlayable* Playable = GetPawn<ACHPlayable>();
	//if pawn is alive check if is in shop
	if (Playable)
	{
		if (!IsInShop())
		{
			UE_LOG(LogCHPlayable, Display, TEXT("Not in shop, cannot sell item"));
			return;
		}
	}

	const APSAllMid* PS = GetPlayerState<APSAllMid>();
	const bool Status = PS->InventoryComponent->RemoveItem(Item);
	if (Status)
	{
		APSAllMid* ThisPlayerState = GetPlayerState<APSAllMid>();
		Item->OnSell(Playable, ThisPlayerState);
	}
	UE_LOG(LogCHPlayable, Display, TEXT("RemoveItem: %s"), Status ? TEXT("True") : TEXT("False"));
}

void APCAllMid::ConsumeItem(UConsumableItem* ConsumableItem)
{
	if (!ConsumableItem) { return; }
	if (!HasAuthority())
	{
		Server_ConsumeItem(ConsumableItem);
		return;
	}
	ACHPlayable* Playable = GetPawn<ACHPlayable>();
	if (!Playable) { return; }

	const APSAllMid* PS = GetPlayerState<APSAllMid>();
	const bool Status = PS->InventoryComponent->ConsumeItem(ConsumableItem, 1);

	if (Status)
	{
		ConsumableItem->Use(Playable);
	}
	UE_LOG(LogCHPlayable, Display, TEXT("ConsumableItem: %s"), Status ? TEXT("True") : TEXT("False"));
}

void APCAllMid::Server_ConsumeItem_Implementation(UConsumableItem* ConsumableItem)
{
	ConsumeItem(ConsumableItem);
}

void APCAllMid::Server_SellItem_Implementation(UBaseItem* Item)
{
	SellItem(Item);
}

void APCAllMid::Server_BuyItem_Implementation(const TSubclassOf<UBaseItem> Item)
{
	BuyItem(Item);
}
