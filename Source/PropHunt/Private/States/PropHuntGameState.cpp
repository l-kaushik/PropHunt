// Fill out your copyright notice in the Description page of Project Settings.


#include "States/PropHuntGameState.h"
#include "Controller/PropHuntPlayerController.h"
#include "Controller/MenuController.h"
#include "States/PropHuntPlayerState.h"
#include "GameInstance/PropHuntGameInstance.h"
#include "Utils/PropHuntLog.h"

#include "Net/UnrealNetwork.h"

const float APropHuntGameState::GAME_TIME_IN_SECONDS = 300.0f;

APropHuntGameState::APropHuntGameState() 
{
	bHasGameStarted = false;
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
	DOREPLIFETIME(APropHuntGameState, MinPlayerNum);
	DOREPLIFETIME(APropHuntGameState, bIsPropWon);
}

// Getters

TArray<APropHuntPlayerController*>& APropHuntGameState::GetPlayerControllerList()
{
	return PlayerControllerList;
}

const TArray<AMenuController*>& APropHuntGameState::GetMenuPlayerControllerList() const
{
	return MenuPlayerControllerList;
}

TArray<APropHuntPlayerController*>& APropHuntGameState::GetHunterList()
{
	return HunterList;
}

const TArray<APropHuntPlayerState*>& APropHuntGameState::GetPlayerStates() const
{
	return PlayerStates;
}

bool APropHuntGameState::GetHasGameStarted() const
{
	return bHasGameStarted;
}

bool APropHuntGameState::GetIsPropWon() const
{
	return bIsPropWon;
}

int32 APropHuntGameState::GetMinPlayerNum() const
{
	return MinPlayerNum;
}

// Setters

void APropHuntGameState::AddPlayerController(APropHuntPlayerController* NewController)
{
	if (NewController) {
		PlayerControllerList.AddUnique(NewController);		
		PlayerStates.AddUnique(NewController->GetPlayerState<APropHuntPlayerState>());
	}
}

void APropHuntGameState::AddMenuController(AMenuController* NewController)
{
	if (NewController)
	{
		UE_LOG(LogPropHuntGlobal, Display, TEXT("New Player Added to the list in game state"));
		MenuPlayerControllerList.AddUnique(NewController);
		PlayerStates.AddUnique(NewController->GetPlayerState<APropHuntPlayerState>());
		OnRep_PlayerStates();	// rep notify doesn't work on server side in cpp, so this explicit call make sure server gets the updated list as well.
	}
}

void APropHuntGameState::RemoveMenuController(AMenuController* ExistingController)
{
	if (ExistingController)
	{
		MenuPlayerControllerList.Remove((ExistingController));
		PlayerStates.Remove(ExistingController->GetPlayerState<APropHuntPlayerState>());
	}
}

void APropHuntGameState::RemovePlayerController(APropHuntPlayerController* ExistingController)
{
	if (ExistingController)
	{
		PlayerControllerList.Remove(ExistingController);
	}
}

void APropHuntGameState::AddHunter(APropHuntPlayerController* NewHunter) {
	if (NewHunter) {
		HunterList.AddUnique(NewHunter);
	}
}

void APropHuntGameState::SetHasGameStarted(bool InHasGameStarted)
{
	bHasGameStarted = InHasGameStarted;
}

void APropHuntGameState::SetIsPropWon(bool InIsPropWon)
{
	bIsPropWon = InIsPropWon;
}

void APropHuntGameState::SetMinPlayerNum(int32 InMinPlayerNum)
{
	MinPlayerNum = InMinPlayerNum;
}

// rep notifiers

void APropHuntGameState::OnRep_PlayerStates()
{
	UE_LOG(LogPropHuntGlobal, Display, TEXT("Broadcasting playerStates, %d"), PlayerStates.Num());
	OnPlayerListUpdated.Broadcast(PlayerStates);
}
