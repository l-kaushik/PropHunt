// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Structs/ScoreboardData.h"
#include "CoreMinimal.h"
#include "MatchData.generated.h"

USTRUCT()
struct FMatchData
{
	GENERATED_BODY();

	bool operator==(const FMatchData& Other) const
	{
		return MatchID == Other.MatchID;
	}

	UPROPERTY(SaveGame)
	FString MatchID;

	UPROPERTY(SaveGame)
	FDateTime MatchStartTimeStamp;
	
	UPROPERTY(SaveGame)
	FDateTime MatchEndTimeStamp;

	UPROPERTY(SaveGame)
	FString MapName;

	UPROPERTY(SaveGame)
	FString HostName;
	
	UPROPERTY(SaveGame)
	FScoreboardData ScoreboardData;
};