// Fill out your copyright notice in the Description page of Project Settings.


#include "PropHuntGameState.h"
#include "PropHuntPlayerController.h"
#include "Net/UnrealNetwork.h"

APropHuntGameState::APropHuntGameState() 
{
	bHasGameStarted = false;
	MinPlayerNum = 2;
	HunterStartLocation.Set(606.749038, 1241.206218, 99.227695);	// for test purposes only
}

void APropHuntGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APropHuntGameState, PlayerControllerList);
	DOREPLIFETIME(APropHuntGameState, bHasGameStarted);
	DOREPLIFETIME(APropHuntGameState, HunterList);
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
