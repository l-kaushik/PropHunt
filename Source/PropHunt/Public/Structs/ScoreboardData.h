// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Structs/PlayerScoreboardData.h"
#include "CoreMinimal.h"
#include "ScoreboardData.generated.h"

USTRUCT()
struct FScoreboardData
{
	GENERATED_BODY()


	UPROPERTY(SaveGame)
	TArray<FPlayerScoreboardData> PlayerStats;

	UPROPERTY(SaveGame)
	FPlayerScoreboardData BestHunter;

	UPROPERTY(SaveGame)
	FPlayerScoreboardData BestProp;
};