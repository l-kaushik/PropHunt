// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"
#include "../Controller/MenuController.h"
#include "../States/PropHuntGameState.h"
#include "../States/PropHuntPlayerState.h"

AMenuGameMode::AMenuGameMode()
{
	PlayerControllerClass = AMenuController::StaticClass();
	GameStateClass = APropHuntGameState::StaticClass();
	PlayerStateClass = APropHuntPlayerState::StaticClass();
	DefaultPawnClass = nullptr;
}

