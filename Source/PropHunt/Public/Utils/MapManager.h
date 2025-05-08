// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utils/MapDataAsset.h"

/**
 * 
 */
class PROPHUNT_API MapManager
{
public:

	static MapManager& Get();

    static FString GetMapWithListen(const TCHAR* MapName);
    static const FMapInfo* GetMapInfo(const FString& MapName);
    static const FString GetMapAddressWithListen(const FString& MapName);
    static const TMap<FString, FMapInfo>& GetAllMaps();
    static const bool IsInitialized();

private:
	MapManager();
    void LoadMapVariables();

public:
    static const TCHAR* Map_Menu;
    static const TCHAR* Map_Warehouse;

private:
    static bool m_IsInitialized;
    
    UPROPERTY()
    static UMapDataAsset* MapDataAsset;
};
