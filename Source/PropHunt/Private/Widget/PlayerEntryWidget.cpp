// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PlayerEntryWidget.h"

#include "Components/TextBlock.h"

void UPlayerEntryWidget::SetPlayerNameText(const FString& InPlayerName)
{
	PlayerNameText->SetText(FText::FromString(InPlayerName));
}

void UPlayerEntryWidget::SetPingText(const FString& InPing)
{
	PingText->SetText(FText::FromString(InPing));
}

void UPlayerEntryWidget::SetReadyStatusText(const FString& InReadyStatus)
{
	ReadyStatusText->SetText(FText::FromString(InReadyStatus));
}
