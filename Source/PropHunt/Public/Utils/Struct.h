// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Struct.generated.h"

USTRUCT(BlueprintType)
struct FMapInfo
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapInfo")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapInfo")
	FString MapPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapInfo")
	FVector HunterSpawnCoordinate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapInfo")
	FVector PropSpawnCoordinate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapInfo")
	UTexture2D* Image;
};