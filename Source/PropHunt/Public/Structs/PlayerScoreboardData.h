// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Structs/PlayerData.h"
#include "CoreMinimal.h"
#include "PlayerScoreboardData.generated.h"

USTRUCT()
struct FPlayerScoreboardData
{
	GENERATED_BODY()

	FPlayerScoreboardData()
	{
		kills = 0;
		Assists = 0;
		DamageGiven = 0;
		DamageTaken = 0;
		HiddenTime = 0.f;
	}

	UPROPERTY(SaveGame)
	FPlayerData PlayerData;

	UPROPERTY(SaveGame)
	int32 kills;	
	
	UPROPERTY(SaveGame)
	int32 Assists;

	UPROPERTY(SaveGame)
	int32 DamageGiven;

	UPROPERTY(SaveGame)
	int32 DamageTaken;

	UPROPERTY(SaveGame)
	double HiddenTime;
};