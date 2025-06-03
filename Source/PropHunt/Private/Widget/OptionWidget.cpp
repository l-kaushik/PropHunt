// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/OptionWidget.h"
#include "Macros/WidgetMacros.h"
#include "Widget/Components/Button/MasterButton.h"
#include "Utils/WidgetUtils.h"
#include "Utils/PropHuntLog.h"

#include "Components/WidgetSwitcher.h"

void UOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindClickEvent();

	// load settings
}

void UOptionWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	InitializeLabels();
}

void UOptionWidget::InitializeLabels()
{
	GameplayButton->SetLabel("Gameplay");
	AudioButton->SetLabel("Audio");
	VideoButton->SetLabel("Video");
}

void UOptionWidget::BindClickEvent()
{
	BIND_BUTTON_CLICK(GameplayButton, &UOptionWidget::OnGameplayButtonClicked);
	BIND_BUTTON_CLICK(AudioButton, &UOptionWidget::OnAudioButtonClicked);
	BIND_BUTTON_CLICK(VideoButton, &UOptionWidget::OnVideoButtonClicked);
}

void UOptionWidget::OnGameplayButtonClicked()
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Gameplay button clicked"));
	MenuSwitcher->SetActiveWidgetIndex(0);
}

void UOptionWidget::OnAudioButtonClicked()
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Audio button clicked"));
	MenuSwitcher->SetActiveWidgetIndex(1);
}

void UOptionWidget::OnVideoButtonClicked()
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Video button clicked"));
	MenuSwitcher->SetActiveWidgetIndex(2);
}

