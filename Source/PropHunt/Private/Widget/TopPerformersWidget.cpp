// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/TopPerformersWidget.h"
#include "Structs/PlayerScoreboardData.h"
#include "Utils/WidgetUtils.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UTopPerformersWidget::SetPropImage(UTexture2D* InImage)
{
	if (!InImage) return;

	FSlateBrush Brush;
	Brush.SetResourceObject(InImage);
	PropImage->SetBrush(Brush);
}

void UTopPerformersWidget::SetPropName(const FString& InName)
{
	PropName->SetText(FText::FromString(InName));
}

void UTopPerformersWidget::SetPropHiddenTime(double InTime)
{
	int32 TotalSeconds = FMath::FloorToInt(InTime);
	int32 Minutes = TotalSeconds / 60;
	int32 Seconds = TotalSeconds % 60;
	FString Time = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	FText TextToShow = FText::FromString(Time);
	PropHiddenTime->SetText(TextToShow);

}

void UTopPerformersWidget::SetHunterImage(UTexture2D* InImage)
{
	if (!InImage) return;

	FSlateBrush Brush;
	Brush.SetResourceObject(InImage);
	HunterImage->SetBrush(Brush);
}

void UTopPerformersWidget::SetHunterName(const FString& InName)
{
	HunterName->SetText(FText::FromString(InName));
}

void UTopPerformersWidget::SetHunterKills(int32 InKills)
{
	FString KillsString = FString::FromInt(InKills);
	FText KillsText = FText::FromString(KillsString);
	HunterKills->SetText(KillsText);
}

void UTopPerformersWidget::SetPropData(FPlayerScoreboardData InPlayerData)
{
	UTexture2D* Image = WidgetUtils::CreateTextureFromRawData(InPlayerData.PlayerData.ProfileImage);

	if (Image) SetPropImage(Image);
	SetPropName(InPlayerData.PlayerData.Username);
	SetPropHiddenTime(InPlayerData.HiddenTime);
}

void UTopPerformersWidget::SetHunterData(FPlayerScoreboardData InHunterData)
{
	UTexture2D* Image = WidgetUtils::CreateTextureFromRawData(InHunterData.PlayerData.ProfileImage);

	if (Image) SetHunterImage(Image);
	SetHunterName(InHunterData.PlayerData.Username);
	SetHunterKills(InHunterData.kills);
}
