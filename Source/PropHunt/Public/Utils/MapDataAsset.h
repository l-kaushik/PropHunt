// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Structs/MapInfo.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MapDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROPHUNT_API UMapDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
    const TMap<FString, FMapInfo>& GetAllMaps() const
    {
        return AllMaps;
    }

private:
    UPROPERTY(EditAnywhere)
    TMap<FString, FMapInfo> AllMaps;
};
