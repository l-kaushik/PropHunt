// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PROPHUNT_API GameConstants
{
public:
	static constexpr const TCHAR* Map_Menu = TEXT("/Game/ThirdPerson/Maps/MenuMap");
	static constexpr const TCHAR* Map_ThirdPerson = TEXT("/Game/ThirdPerson/Maps/ThirdPersonMap");
	static constexpr const TCHAR* Map_Warehouse = TEXT("/Game/ThirdPerson/Maps/WarehouseMap");

	static FString GetMapWithListen(const TCHAR* MapName) {
		return FString::Printf(TEXT("%s?listen"), MapName);
	}
};
