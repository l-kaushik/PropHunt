// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerData.generated.h"

USTRUCT()
struct FPlayerData
{
	GENERATED_BODY();

	UPROPERTY(SaveGame)
	FString Username;

	UPROPERTY(SaveGame)
	FString UserID;

	UPROPERTY(SaveGame)
	TArray<uint8> ProfileImageBytes;
};