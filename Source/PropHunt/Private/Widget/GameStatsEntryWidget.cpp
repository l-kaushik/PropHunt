// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/GameStatsEntryWidget.h"
#include "Structs/PlayerScoreboardData.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

const FLinearColor UGameStatsEntryWidget::Backgroundcolor = FLinearColor(1.f, 1.f, 1.f, 0.1f);

UGameStatsEntryWidget* UGameStatsEntryWidget::SetPlayerName(const FString& InPlayerName)
{
    FString newName;
    if (InPlayerName.Len() > 15)
    {
        newName = InPlayerName.Left(15 - 3) + TEXT("...");
    }

    PlayerName->SetText(FText::FromString(newName));
    return this;
}

UGameStatsEntryWidget* UGameStatsEntryWidget::SetKills(const FString& InKills)
{
    Kills->SetText(FText::FromString(InKills));
    return this;
}

UGameStatsEntryWidget* UGameStatsEntryWidget::SetAssists(const FString& InAssists)
{
    Assists->SetText(FText::FromString(InAssists));
    return this;
}

UGameStatsEntryWidget* UGameStatsEntryWidget::SetDamageGiven(const FString& InDamageGiven)
{
    DamageGiven->SetText(FText::FromString(InDamageGiven));
    return this;
}

UGameStatsEntryWidget* UGameStatsEntryWidget::SetDamageTaken(const FString& InDamageTaken)
{
    DamageTaken->SetText(FText::FromString(InDamageTaken));
    return this;
}

void UGameStatsEntryWidget::ClearStats()
{
    PlayerName->SetText(FText::GetEmpty());
    Kills->SetText(FText::GetEmpty());
    Assists->SetText(FText::GetEmpty());
    DamageGiven->SetText(FText::GetEmpty());
    DamageTaken->SetText(FText::GetEmpty());
}

void UGameStatsEntryWidget::SetData(FPlayerScoreboardData InPlayerScoreboardData)
{
    SetPlayerName(InPlayerScoreboardData.PlayerData.Username);
    SetKills(FString::FromInt(InPlayerScoreboardData.kills));
    SetAssists(FString::FromInt(InPlayerScoreboardData.Assists));
    SetDamageGiven(FString::FromInt(InPlayerScoreboardData.DamageGiven));
    SetDamageTaken(FString::FromInt(InPlayerScoreboardData.DamageTaken));
}

void UGameStatsEntryWidget::NativePreConstruct()
{
    if (!Background || bBackgroundDisabled) return;
    Background->SetColorAndOpacity(Backgroundcolor);
}

void UGameStatsEntryWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // little lighter shade
    static const FLinearColor LighterShade = FLinearColor(1.f, 1.f, 1.f, 0.2f);
    if (!Background || bBackgroundDisabled) return;
    Background->SetColorAndOpacity(LighterShade);
}

void UGameStatsEntryWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    if (!Background || bBackgroundDisabled) return;
    Background->SetColorAndOpacity(Backgroundcolor);
}


