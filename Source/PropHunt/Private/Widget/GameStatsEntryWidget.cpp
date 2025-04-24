// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/GameStatsEntryWidget.h"

#include "Components/TextBlock.h"

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

