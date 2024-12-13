// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ServerEntryWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

void UServerEntryWidget::SetServerNameText(const FString& InSessionName)
{
	ServerNameText->SetText(FText::FromString(InSessionName));
}

void UServerEntryWidget::SetPingText(const FString& InPing)
{
	PingText->SetText(FText::FromString(InPing));
}

void UServerEntryWidget::SetStatusText(const FString& InStatus)
{
	StatusText->SetText(FText::FromString(InStatus));
}
