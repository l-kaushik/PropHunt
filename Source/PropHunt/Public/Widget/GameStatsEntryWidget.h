// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStatsEntryWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class PROPHUNT_API UGameStatsEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UGameStatsEntryWidget* SetPlayerName(const FString& InPlayerName);
	UGameStatsEntryWidget* SetKills(const FString& InKills = "0");
	UGameStatsEntryWidget* SetAssists(const FString& InAssists = "0");
	UGameStatsEntryWidget* SetDamageGiven(const FString& InDamageGiven = "0");
	UGameStatsEntryWidget* SetDamageTaken(const FString& InDamageTaken = "0");
	void ClearStats();

public:
	virtual void NativePreConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:

	// design
	UPROPERTY(meta = (BindWidget))
	UImage* Background;

	static const FLinearColor Backgroundcolor;

	// stats

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Kills;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Assists;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DamageGiven;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DamageTaken;
};
