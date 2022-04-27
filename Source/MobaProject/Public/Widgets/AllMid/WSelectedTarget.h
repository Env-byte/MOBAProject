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
	UPROPERTY(BlueprintReadOnly)
	UCHAttributeSet* Attributes;
	
	void SetTarget(UCHAttributeSet* ThisAttributes, FName Name);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnAttributeChange();
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BP_SetTarget(FName Name);
public:
	UFUNCTION(BlueprintImplementableEvent)
	void Hide();
	UFUNCTION(BlueprintImplementableEvent)
	void Show();
};
