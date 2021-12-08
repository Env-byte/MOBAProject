// Copyright Epic Games, Inc. All Rights Reserved.

#include "MobaProjectGameMode.h"
#include "MobaProjectPlayerController.h"
#include "MobaProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMobaProjectGameMode::AMobaProjectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AMobaProjectPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}