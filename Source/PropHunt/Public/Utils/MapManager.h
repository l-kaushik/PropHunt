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

	static FString GetMapWithListen(const TCHAR* MapName) {
		return FString::Printf(TEXT("%s?listen"), MapName);
	}

    static const FMapInfo* GetMapInfo(const FString& MapName)
    {
        if (const FMapInfo* MapInfo = AllMaps.Find(MapName))
        {
            return MapInfo;
        }

        return nullptr;
    }

    static const FString GetMapAddressWithListen(const FString& MapName)
    {
        if (const FMapInfo* MapInfo = GetMapInfo(MapName))
        {
            return FString::Printf(TEXT("%s?listen"), *MapInfo->MapPath);
        }

        return FString();
    }

private:
	MapManager();

	void LoadAllMaps();
    void LoadMapVariables();

public:
    static const TCHAR* Map_Menu;
    static const TCHAR* Map_Warehouse;

private:
    static TMap<FString, FMapInfo> AllMaps;
};
