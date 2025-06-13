// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsData.generated.h"

USTRUCT()
struct FSettingsData
{
	GENERATED_BODY();

	FSettingsData()
	{
		CameraSensitivity = 50;
		MusicVolume = 50;
		SFXVolume = 50;
		ScreenPercentage = 100;
		AntiAliasingMethod = 0;
	}

	UPROPERTY(SaveGame)
	FString OverallGraphics;

	UPROPERTY(SaveGame)
	float CameraSensitivity;

	UPROPERTY(SaveGame)
	float MusicVolume;

	UPROPERTY(SaveGame)
	float SFXVolume;

	UPROPERTY(SaveGame)
	float ScreenPercentage;

	UPROPERTY(SaveGame)
	int32 AntiAliasingMethod;

};