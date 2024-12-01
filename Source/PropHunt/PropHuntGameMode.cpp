// Copyright Epic Games, Inc. All Rights Reserved.

#include "PropHuntGameMode.h"
#include "PropHuntCharacter.h"
#include "PropHuntPlayerController.h"
#include "UObject/ConstructorHelpers.h"

APropHuntGameMode::APropHuntGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_PropCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		PlayerControllerClass = APropHuntPlayerController::StaticClass();
	}
}
