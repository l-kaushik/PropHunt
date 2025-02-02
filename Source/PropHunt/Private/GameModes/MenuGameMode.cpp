// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/MenuGameMode.h"
#include "Controller/MenuController.h"
#include "States/PropHuntGameState.h"
#include "States/PropHuntPlayerState.h"

AMenuGameMode::AMenuGameMode()
{
	PlayerControllerClass = AMenuController::StaticClass();
	GameStateClass = APropHuntGameState::StaticClass();
	PlayerStateClass = APropHuntPlayerState::StaticClass();
	DefaultPawnClass = nullptr;
	bUseSeamlessTravel = true;
}

void AMenuGameMode::PostLogin(APlayerController* InNewPlayer)
{
	Super::PostLogin(InNewPlayer);

	PropHuntGameState = GetGameState<APropHuntGameState>();

	if (PropHuntGameState)
	{
		auto* NewPlayer = Cast<AMenuController>(InNewPlayer);
		PropHuntGameState->AddMenuController(NewPlayer);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("MyGameState is invalid"));
	}
}

void AMenuGameMode::Logout(AController* ExistingPlayer)
{
	Super::Logout(ExistingPlayer);

	PropHuntGameState->GetMenuPlayerControllerList().Remove(Cast<AMenuController>(ExistingPlayer));
}

void AMenuGameMode::StartGame()
{
	GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
}
