// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UPropHuntSaveGame;

class PROPHUNT_API SaveGameManager
{
public:
	static SaveGameManager& Get();

	void SaveGame(UPropHuntSaveGame* SaveGameInstance, const FString& SlotName);
	UPropHuntSaveGame* LoadGame(const FString& SlotName);
	FString& GetLastSaveGameSlotName();
	
private:
	SaveGameManager();
	static UPropHuntSaveGame* MetaSaveGameInstance;
};
