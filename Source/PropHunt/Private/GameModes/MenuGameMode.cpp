// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/MenuGameMode.h"
#include "Controller/MenuController.h"
#include "States/PropHuntGameState.h"
#include "States/PropHuntPlayerState.h"
#include "GameInstance/PropHuntGameInstance.h"
#include "Utils/PropHuntLog.h"

AMenuGameMode::AMenuGameMode()
{
	PlayerControllerClass = AMenuController::StaticClass();
	GameStateClass = APropHuntGameState::StaticClass();
	PlayerStateClass = APropHuntPlayerState::StaticClass();
	DefaultPawnClass = nullptr;
}

void AMenuGameMode::PostLogin(APlayerController* InNewPlayer)
{
	Super::PostLogin(InNewPlayer);

	PropHuntGameState = GetGameState<APropHuntGameState>();
	PropHuntGameInstance = GetGameInstance<UPropHuntGameInstance>();

	if (PropHuntGameState)
	{
		auto* NewPlayer = Cast<AMenuController>(InNewPlayer);
		PropHuntGameState->AddMenuController(NewPlayer);
		PropHuntGameInstance->RegisterPlayer(GetUniqueIdFromController(NewPlayer));
	}
	else {
		UE_LOG(LogPropHuntMenuGameMode, Warning, TEXT("MyGameState is invalid"));
	}
}

void AMenuGameMode::Logout(AController* ExistingPlayer)
{
	Super::Logout(ExistingPlayer);

	auto* Player = Cast<AMenuController>(ExistingPlayer);
	PropHuntGameState->RemoveMenuController(Player);
	PropHuntGameInstance->UnregisterPlayer(GetUniqueIdFromController(Player));
}

void AMenuGameMode::StartGame()
{
	GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
}

const FUniqueNetId& AMenuGameMode::GetUniqueIdFromController(AMenuController* Player)
{
	auto* PlayerState = Player->GetPlayerState<APropHuntPlayerState>();
	return *PlayerState->GetUniqueId();
}
