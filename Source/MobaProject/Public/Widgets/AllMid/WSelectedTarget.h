// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/CHAttributeSet.h"
#include "WSelectedTarget.generated.h"

/**
 * 
 */
UCLASS()
class MOBAPROJECT_API UWSelectedTarget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetTarget(UCHAttributeSet* Attributes, FName Name, int32 Level);
	
	UFUNCTION(BlueprintImplementableEvent)
	void Hide();
	UFUNCTION(BlueprintImplementableEvent)
	void Show();
};
