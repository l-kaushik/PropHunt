// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreboardMenuWidget.generated.h"

class UMasterButton;
class UWidgetSwitcher;
class UGameStatsWidget;
class UTopPerformersWidget;
class UGameStatsEntryWidget;

UCLASS()
class PROPHUNT_API UScoreboardMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddPlayerStatsToList(UGameStatsEntryWidget* GameStateEntryWidget);
	void ClearPlayerStatsList();

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

private:
	void OnGameStatsButtonClicked();
	void OnTopPerformerButtonClicked();
	void SwitchScoreboardMenuButtonsProperty(bool GameStatsButtonClicked);
private:
	// scoreboard menu button
	UPROPERTY(meta = (BindWidget))
	UMasterButton* GameStatsButton;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* TopPerformerButton;

	// scoreboard menu switcher
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* ScoreboardMenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UGameStatsWidget* GameStatsPlayerListWindow;

	UPROPERTY(meta = (BindWidget))
	UTopPerformersWidget* TopPerformerWindow;
};
