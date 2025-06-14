// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImageData.generated.h"

USTRUCT()
struct FImageData
{
	GENERATED_BODY()

	FImageData()
	{
		Width = 0;
		Height = 0;
	}

	UPROPERTY(SaveGame)
	int32 Width;

	UPROPERTY(SaveGame)
	int32 Height;

	UPROPERTY(SaveGame)
	TArray<uint8> RawBytes;
};