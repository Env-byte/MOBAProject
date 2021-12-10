// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterNamePlate.h"

#include "Widgets/Components//WNamePlate.h"

UCharacterNamePlate::UCharacterNamePlate()
{
	SetRelativeRotation(FRotator::ZeroRotator);
	// set widget class to our Blueprinted widget class
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetComponentBPClass(TEXT("/Game/Blueprints/Widgets/Components/BPW_NamePlate"));
	if (WidgetComponentBPClass.Class != nullptr)
	{
		WidgetClass = WidgetComponentBPClass.Class;
	}
	Space = EWidgetSpace::Screen;
	bDrawAtDesiredSize = true;
}

UWNamePlate* UCharacterNamePlate::GetNamePlateWidget() const
{
	UWidget* ThisWidget = GetWidget();
	if (ThisWidget)
	{
		return Cast<UWNamePlate>(ThisWidget);
	}
	return nullptr;
}
