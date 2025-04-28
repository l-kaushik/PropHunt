// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStatsWidget.generated.h"

class UVerticalBox;
class UGameStatsEntryWidget;

UCLASS()
class PROPHUNT_API UGameStatsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddPlayerStatsToList(UGameStatsEntryWidget* PlayerStats);
	void ClearList();

private:

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBox;
};
