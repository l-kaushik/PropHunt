// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Structs/ImageData.h"
#include "CoreMinimal.h"
#include "PlayerData.generated.h"

USTRUCT()
struct FPlayerData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	FString Username;

	UPROPERTY(SaveGame)
	FString UserID;

	UPROPERTY(SaveGame)
	FImageData ProfileImage;
};