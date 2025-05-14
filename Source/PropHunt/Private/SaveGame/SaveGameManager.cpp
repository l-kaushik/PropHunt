// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/SaveGameManager.h"
#include "SaveGame/PropHuntSaveGame.h"

#include "Kismet/GameplayStatics.h"

UPropHuntSaveGame* SaveGameManager::MetaSaveGameInstance = nullptr;

SaveGameManager& SaveGameManager::Get()
{
	static SaveGameManager Instance;
	return Instance;
}

void SaveGameManager::SaveGame(UPropHuntSaveGame* SaveGameInstance, const FString& SlotName)
{
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);

    // save the last saved slot name
    MetaSaveGameInstance->PlayerData.Username = SlotName;
    UGameplayStatics::SaveGameToSlot(MetaSaveGameInstance, TEXT("LastUsedSlot"), 0);
}

UPropHuntSaveGame* SaveGameManager::LoadGame(const FString& SlotName)
{
    UPropHuntSaveGame* SaveGameInstance = nullptr;

    if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
    {
        SaveGameInstance = Cast<UPropHuntSaveGame>(
            UGameplayStatics::LoadGameFromSlot(SlotName, 0));
    }

    // If save doesn't exist, return a new instance
    if (!SaveGameInstance)
    {
        SaveGameInstance = Cast<UPropHuntSaveGame>(UGameplayStatics::CreateSaveGameObject(UPropHuntSaveGame::StaticClass()));
    }

    return SaveGameInstance;
}

FString& SaveGameManager::GetLastSaveGameSlotName()
{
    return MetaSaveGameInstance->PlayerData.Username;
}

SaveGameManager::SaveGameManager()
{
    MetaSaveGameInstance = LoadGame(TEXT("LastUsedSlot"));
}