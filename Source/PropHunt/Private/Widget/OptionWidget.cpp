// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/OptionWidget.h"
#include "Widget/Components/Button/MasterButton.h"
#include "Widget/Components/SelectionBox.h"
#include "Macros/WidgetMacros.h"
#include "Utils/WidgetUtils.h"
#include "Utils/PropHuntLog.h"

#include "Components/WidgetSwitcher.h"
#include "Components/Slider.h"

void UOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindClickEvent();
	BindSliderEvents();
	BindSelectionBoxEvents();

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

void UOptionWidget::BindSliderEvents()
{
	CameraSensitivitySlider->OnValueChanged.AddDynamic(this, &UOptionWidget::OnCameraSensitivityChanged);
	MusicVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionWidget::OnMusicVolumeChanged);
	SFXVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionWidget::OnSFXVolumeChanged);
}

void UOptionWidget::BindSelectionBoxEvents()
{
	OverallGraphicsSelectionBox->OnSelectionChanged.AddUObject(this, &UOptionWidget::OnOverallGraphicsChanged);
	TextureQualitySelectionBox->OnSelectionChanged.AddUObject(this, &UOptionWidget::OnTextureQualityChanged);
	ShadowQualitySelectionBox->OnSelectionChanged.AddUObject(this, &UOptionWidget::OnShadowQualityChanged);
	ViewDistanceSelectionBox->OnSelectionChanged.AddUObject(this, &UOptionWidget::OnViewDistanceChanged);
	AntiAliasingSelectionBox->OnSelectionChanged.AddUObject(this, &UOptionWidget::OnAntiAliasingChanged);
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

void UOptionWidget::OnCameraSensitivityChanged(float NewValue)
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Camera Sensitivity changed to %f"), NewValue);
}

void UOptionWidget::OnMusicVolumeChanged(float NewValue)
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Music Volume changed to %f"), NewValue);
}

void UOptionWidget::OnSFXVolumeChanged(float NewValue)
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("SFX Volume changed to %f"), NewValue);
}

void UOptionWidget::OnOverallGraphicsChanged(const FString& NewOption)
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Overall Graphics changed to %s"), *NewOption);
}

void UOptionWidget::OnTextureQualityChanged(const FString& NewOption)
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Texture Quality changed to %s"), *NewOption);
}

void UOptionWidget::OnShadowQualityChanged(const FString& NewOption)
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Shadow Quality changed to %s"), *NewOption);
}

void UOptionWidget::OnViewDistanceChanged(const FString& NewOption)
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("View Distance changed to %s"), *NewOption);
}

void UOptionWidget::OnAntiAliasingChanged(const FString& NewOption)
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Anti Aliasing changed to %s"), *NewOption);
}
