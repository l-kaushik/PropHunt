// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/MapManager.h"
#include "Utils/PropHuntLog.h"
#include "Utils/Struct.h"

// define static variables
const TCHAR* MapManager::Map_Menu = nullptr;
const TCHAR* MapManager::Map_Warehouse = nullptr;
TMap<FString, FMapInfo> MapManager::AllMaps;

MapManager& MapManager::Get()
{
    static MapManager s_MapManager;
    return s_MapManager;
}

const TMap<FString, FMapInfo>& MapManager::GetAllMaps()
{
    return AllMaps;
}

MapManager::MapManager()
{
    LoadAllMaps();
}

void MapManager::LoadAllMaps()
{
    UMapDataAsset* MapDataAsset = Cast<UMapDataAsset>(StaticLoadObject(
        UMapDataAsset::StaticClass(),
        nullptr,
        TEXT("/Game/DataAsset/DA_MapDataAsset.DA_MapDataAsset")
    ));

    if (!MapDataAsset)
    {
        UE_LOG(LogPropHuntGlobal, Error, TEXT("Failed to get MapDataAsset"));
        return;
    }

    AllMaps = MapDataAsset->GetAllMaps();
    LoadMapVariables();
}

void MapManager::LoadMapVariables()
{
    Map_Menu = *AllMaps["MenuMap"].MapPath;
    Map_Warehouse = *AllMaps["WarehouseMap"].MapPath;

    UE_LOG(LogTemp, Warning, TEXT("MapManager: %s, %s"), Map_Menu, Map_Warehouse);
}

