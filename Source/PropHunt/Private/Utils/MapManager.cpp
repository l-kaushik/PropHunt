// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/MapManager.h"
#include "Utils/PropHuntLog.h"
#include "Structs/MapInfo.h"

// define static variables
const TCHAR* MapManager::Map_Menu = nullptr;
const TCHAR* MapManager::Map_Warehouse = nullptr;
UMapDataAsset* MapManager::MapDataAsset = nullptr;
bool MapManager::m_IsInitialized = false;

MapManager& MapManager::Get()
{
    static MapManager s_MapManager;
    return s_MapManager;
}

FString MapManager::GetMapWithListen(const TCHAR* MapName)
{
    return FString::Printf(TEXT("%s?listen"), MapName);
}

const FMapInfo* MapManager::GetMapInfo(const FString& MapName)
{
    if (const FMapInfo* MapInfo = MapDataAsset->GetAllMaps().Find(MapName))
    {
        return MapInfo;
    }

    return nullptr;
}

const FString MapManager::GetMapAddressWithListen(const FString& MapName)
{
    if (const FMapInfo* MapInfo = GetMapInfo(MapName))
    {
        return FString::Printf(TEXT("%s?listen"), *MapInfo->MapPath);
    }

    return FString();
}

const TMap<FString, FMapInfo>& MapManager::GetAllMaps()
{
    return MapDataAsset->GetAllMaps();
}

const bool MapManager::IsInitialized()
{
    return m_IsInitialized;
}

MapManager::MapManager()
{
    MapDataAsset = LoadObject<UMapDataAsset>(nullptr, TEXT("/Game/DataAsset/DA_MapDataAsset.DA_MapDataAsset"));

    if (!MapDataAsset)
    {
        UE_LOG(LogPropHuntGlobal, Error, TEXT("Failed to get MapDataAsset"));
        return;
    }

    m_IsInitialized = true;

    LoadMapVariables();
}

void MapManager::LoadMapVariables()
{
    Map_Menu = *MapDataAsset->GetAllMaps()["MenuMap"].MapPath;
    Map_Warehouse = *MapDataAsset->GetAllMaps()["WarehouseMap"].MapPath;

    UE_LOG(LogTemp, Warning, TEXT("MapManager: %s, %s"), Map_Menu, Map_Warehouse);
}

