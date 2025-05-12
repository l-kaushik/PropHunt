// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Structs/PlayerData.h"
#include "CoreMinimal.h"
#include "PlayerScoreboardData.generated.h"

USTRUCT()
struct FPlayerScoreboardData
{
	GENERATED_BODY();

	UPROPERTY(SaveGame)
	FPlayerData PlayerData;

	UPROPERTY(SaveGame)
	int32 kills;

	UPROPERTY(SaveGame)
	int32 DamageGive;

	UPROPERTY(SaveGame)
	int32 DamageTaken;

	UPROPERTY(SaveGame)
	double HiddenTime;
};