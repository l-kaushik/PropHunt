// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/GameStatsWidget.h"
#include "Components/VerticalBox.h"
#include "Widget/GameStatsEntryWidget.h"

void UGameStatsWidget::AddPlayerStatsToList(UGameStatsEntryWidget* PlayerStats)
{
	VerticalBox->AddChild(PlayerStats);
}

void UGameStatsWidget::ClearList()
{
	VerticalBox->ClearChildren();
}
