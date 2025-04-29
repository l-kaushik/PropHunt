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

void UPlayerEntryWidget::SetReadyStatus(const bool IsReady)
{
	FLinearColor Color = IsReady ? FLinearColor(0.f, 1.f, 0.f) : FLinearColor(1.f, 0.f, 0.f);
	FString ReadyText = IsReady ? FString("Ready") : FString("Unready");
	ReadyStatusText->SetText(FText::FromString(ReadyText));
	ReadyStatusText->SetColorAndOpacity(Color);
}
