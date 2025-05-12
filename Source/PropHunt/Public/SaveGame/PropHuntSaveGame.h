// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Structs/PlayerData.h"
#include "Structs/MatchData.h"
#include "Structs/SettingsData.h"
#include "Structs/MatchHistoryMap.h"

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PropHuntSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PROPHUNT_API UPropHuntSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// Data related to this player example:- username, player id, profile image
	UPROPERTY(SaveGame)
	FPlayerData PlayerData;

	// Data related to every matches played by this player
	UPROPERTY(SaveGame)
	FMatchHistoryMap MatchData;

	// Game settings of this player
	UPROPERTY(SaveGame)
	FSettingsData Settings;
	
};
