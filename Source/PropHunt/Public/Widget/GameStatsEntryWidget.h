// Fill out your copyright notice in the Description page of Project Settings.

/*
* TODO: Implement a common parent class for both PlayerEntryWidget and GameStatsEntryWidget
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStatsEntryWidget.generated.h"

class UImage;
class UTextBlock;

struct FPlayerScoreboardData;

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
	void SetData(FPlayerScoreboardData InPlayerScoreboardData);

public:
	virtual void NativePreConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Settings")
	bool bBackgroundDisabled;

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
