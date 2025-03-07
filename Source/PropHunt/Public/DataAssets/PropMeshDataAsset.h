// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/StaticMesh.h"
#include "PropMeshDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FPropData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CapsuleHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CapsuleRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MeshLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> StaticMesh;
};

UCLASS()
class PROPHUNT_API UPropMeshDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TObjectPtr<UStaticMesh>, FPropData> PropDataMap;
};
