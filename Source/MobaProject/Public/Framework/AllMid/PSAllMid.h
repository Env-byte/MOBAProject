// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PSAllMid.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS(Abstract)
class MOBAPROJECT_API APSAllMid : public APlayerState
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
public:
	APSAllMid();

	UPROPERTY(BlueprintReadOnly)
	UInventoryComponent* InventoryComponent;
};
