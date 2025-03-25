// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LoadingScreenWidget.h"
#include "Components/TextBlock.h"

void ULoadingScreenWidget::SetLoadingMessage(const FString& InMessage)
{
	LoadingMessage->SetText(FText::FromString(InMessage));
}
