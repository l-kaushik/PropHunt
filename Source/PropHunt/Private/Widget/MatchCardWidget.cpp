// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MatchCardWidget.h"
#include "Widget/ScoreboardMenuWidget.h"
#include "Utils/PropHuntLog.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

const FLinearColor UMatchCardWidget::BorderTint = FLinearColor(1.f, 1.f, 1.f, 0.4f);

void UMatchCardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MatchDetail->OnClicked.AddDynamic(this, &UMatchCardWidget::OnMatchDetailClicked);
}

void UMatchCardWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	Border->SetBrushColor(BorderTint);
	IsScoreboardMenuVisible = false;
	UpdateScoreboardToggleState();
}

void UMatchCardWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	const static FLinearColor NewBorderTint = FLinearColor(1.f, 1.f, 1.f, 0.8f);
	Border->SetBrushColor(NewBorderTint);
}

void UMatchCardWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	Border->SetBrushColor(BorderTint);
}

void UMatchCardWidget::OnMatchDetailClicked()
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Match detail button clicked!"));

	IsScoreboardMenuVisible = !IsScoreboardMenuVisible;
	UpdateScoreboardToggleState();
}

void UMatchCardWidget::UpdateScoreboardToggleState()
{
	MatchDetailLabel->SetText(FText::FromString(
		IsScoreboardMenuVisible ?  TEXT("▲") : TEXT("▼")));
	ScoreboardMenu->SetVisibility(
		IsScoreboardMenuVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}
