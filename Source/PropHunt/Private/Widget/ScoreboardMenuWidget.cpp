// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ScoreboardMenuWidget.h"
#include "Widget/GameStatsEntryWidget.h"
#include "Widget/GameStatsWidget.h"
#include "Widget/TopPerformersWidget.h"
#include "Widget/Components/Button/MasterButton.h"
#include "Widget/UIManager.h"
#include "Macros/WidgetMacros.h"
#include "Structs/ScoreboardData.h"
#include "Structs/PlayerScoreboardData.h"
#include "Utils/WidgetUtils.h"

#include "Components/WidgetSwitcher.h"

void UScoreboardMenuWidget::AddPlayerStatsToList(UGameStatsEntryWidget* GameStateEntryWidget)
{
	GameStatsPlayerListWindow->AddPlayerStatsToList(GameStateEntryWidget);
}

void UScoreboardMenuWidget::ClearPlayerStatsList()
{
	GameStatsPlayerListWindow->ClearList();
}

void UScoreboardMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BIND_BUTTON_CLICK(GameStatsButton, &UScoreboardMenuWidget::OnGameStatsButtonClicked);
	BIND_BUTTON_CLICK(TopPerformerButton, &UScoreboardMenuWidget::OnTopPerformerButtonClicked);
}

void UScoreboardMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	// set button labels
	GameStatsButton->SetLabel(FString("Game Stats"));
	TopPerformerButton->SetLabel(FString("Top Performers"));

	// set initiali color
	GameStatsButton->SetBackgroundColor(FLinearColor::Blue);
}

void UScoreboardMenuWidget::SetData(FScoreboardData InScoreboardData)
{
	// remove all previous entries
	GameStatsPlayerListWindow->ClearList();

	for (const auto& PlayerStats : InScoreboardData.PlayerStats)
	{
		auto* GameStatsEntryWidgetRef = WidgetUtils::CreateAndValidateWidget<UGameStatsEntryWidget>(this, UUIManager::Get()->GameStatsEntryWidgetBPClassRef);

		GameStatsEntryWidgetRef->SetData(PlayerStats);

		// add player data to list
		GameStatsPlayerListWindow->AddPlayerStatsToList(GameStatsEntryWidgetRef);
	}

	// load top performers data
}

void UScoreboardMenuWidget::OnGameStatsButtonClicked()
{
	SwitchScoreboardMenuButtonsProperty(true);
}

void UScoreboardMenuWidget::OnTopPerformerButtonClicked()
{
	SwitchScoreboardMenuButtonsProperty(false);
}

void UScoreboardMenuWidget::SwitchScoreboardMenuButtonsProperty(bool GameStatsButtonClicked)
{
	static FLinearColor BlueColor = FLinearColor::Blue;
	static FLinearColor GreyShade = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);

	GameStatsButton->SetBackgroundColor(GameStatsButtonClicked ? BlueColor : GreyShade);
	TopPerformerButton->SetBackgroundColor(!GameStatsButtonClicked ? BlueColor : GreyShade);
	ScoreboardMenuSwitcher->SetActiveWidgetIndex(!GameStatsButtonClicked);
}
