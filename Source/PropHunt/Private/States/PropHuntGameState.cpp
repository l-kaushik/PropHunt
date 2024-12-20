// Fill out your copyright notice in the Description page of Project Settings.


#include "States/PropHuntGameState.h"
#include "Controller/PropHuntPlayerController.h"
#include "Controller/MenuController.h"
#include "States/PropHuntPlayerState.h"
#include "Net/UnrealNetwork.h"

const float APropHuntGameState::GAME_TIME_IN_SECONDS = 300.0f;

APropHuntGameState::APropHuntGameState() 
{
	bHasGameStarted = false;
	MinPlayerNum = 2;
	HunterStartLocation.Set(606.749038, 1241.206218, 99.227695);	// for test purposes only
	bIsPropWon = false;
}

void APropHuntGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APropHuntGameState, PlayerControllerList);
	DOREPLIFETIME(APropHuntGameState, bHasGameStarted);
	DOREPLIFETIME(APropHuntGameState, HunterList);
	DOREPLIFETIME(APropHuntGameState, MenuPlayerControllerList);
	DOREPLIFETIME(APropHuntGameState, PlayerStates);
}

void APropHuntGameState::AddPlayerController(APropHuntPlayerController* NewController)
{
	if (NewController) {
		PlayerControllerList.AddUnique(NewController);
	}
}

TArray<APropHuntPlayerController*> APropHuntGameState::GetPlayerControllerList() const
{
	return PlayerControllerList;
}

void APropHuntGameState::AddHunter(APropHuntPlayerController* NewHunter) {
	if (NewHunter) {
		HunterList.AddUnique(NewHunter);
	}
}

void APropHuntGameState::AddMenuController(AMenuController* NewController)
{
	if (NewController)
	{
		MenuPlayerControllerList.AddUnique(NewController);
		PlayerStates.AddUnique(NewController->GetPlayerState<APropHuntPlayerState>());
		OnRep_PlayerStates();	// rep notify doesn't work on server side in cpp, so this explicit call make sure server gets the updated list as well.
	}
}

void APropHuntGameState::OnRep_PlayerStates()
{
	OnPlayerListUpdated.Broadcast(PlayerStates);
}
