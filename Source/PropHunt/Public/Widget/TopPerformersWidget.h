// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopPerformersWidget.generated.h"

class UImage;
class UTextBlock;

struct FPlayerScoreboardData;

UCLASS()
class PROPHUNT_API UTopPerformersWidget : public UUserWidget
{

	GENERATED_BODY()

public:
	void SetPropImage(UTexture2D* InImage);
	void SetPropName(const FString& InName);
	void SetPropHiddenTime(double InTime);

	void SetHunterImage(UTexture2D* InImage);
	void SetHunterName(const FString& InName);
	void SetHunterKills(int32 InKills);

	void SetPropData(FPlayerScoreboardData InPlayerData);
	void SetHunterData(FPlayerScoreboardData InHunterData);

private:

	// prop data
	UPROPERTY(meta = (BindWidget))
	UImage* PropImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PropName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PropHiddenTime;

	// hunter data
	UPROPERTY(meta = (BindWidget))
	UImage* HunterImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HunterName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HunterKills;
};
