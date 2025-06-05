// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/OptionWidget.h"
#include "Widget/Components/Button/MasterButton.h"
#include "Widget/Components/SelectionBox.h"
#include "Macros/WidgetMacros.h"
#include "Utils/WidgetUtils.h"
#include "Utils/PropHuntLog.h"
#include "Controller/PropHuntPlayerController.h"

#include "Components/WidgetSwitcher.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// load controller
	PlayerController = GetOwningPlayer<APropHuntPlayerController>();

	BindClickEvent();
	BindSliderEvents();
	BindSelectionBoxEvents();

	// load values
	CameraSensitivitySlider->SetValue(50.f);
	MusicVolumeSlider->SetValue(50.f);
	SFXVolumeSlider->SetValue(50.f);
}

void UOptionWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	InitializeLabels();
	InitializeSliders();
}

void UOptionWidget::InitializeLabels()
{
	GameplayButton->SetLabel("Gameplay");
	AudioButton->SetLabel("Audio");
	VideoButton->SetLabel("Video");
}

void UOptionWidget::InitializeSliders()
{
	CameraSensitivitySlider->SetMinValue(1.f);
	CameraSensitivitySlider->SetMaxValue(100.f);
	MusicVolumeSlider->SetMinValue(0.f);
	MusicVolumeSlider->SetMaxValue(100.f);
	SFXVolumeSlider->SetMinValue(0.f);
	SFXVolumeSlider->SetMaxValue(100.f);
}

void UOptionWidget::BindClickEvent()
{
	BIND_BUTTON_CLICK(GameplayButton, &UOptionWidget::OnGameplayButtonClicked);
	BIND_BUTTON_CLICK(AudioButton, &UOptionWidget::OnAudioButtonClicked);
	BIND_BUTTON_CLICK(VideoButton, &UOptionWidget::OnVideoButtonClicked);
}

void UOptionWidget::BindSliderEvents()
{
	CameraSensitivitySlider->OnValueChanged.RemoveDynamic(this, &UOptionWidget::OnCameraSensitivityChanged);
	CameraSensitivitySlider->OnValueChanged.AddDynamic(this, &UOptionWidget::OnCameraSensitivityChanged);

	MusicVolumeSlider->OnValueChanged.RemoveDynamic(this, &UOptionWidget::OnMusicVolumeChanged);
	MusicVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionWidget::OnMusicVolumeChanged);

	SFXVolumeSlider->OnValueChanged.RemoveDynamic(this, &UOptionWidget::OnSFXVolumeChanged);
	SFXVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionWidget::OnSFXVolumeChanged);

}

void UOptionWidget::BindSelectionBoxEvents()
{
	OverallGraphicsSelectionBox->OnSelectionChanged.RemoveAll(this);
	OverallGraphicsSelectionBox->OnSelectionChanged.AddUObject(this, &UOptionWidget::OnOverallGraphicsChanged);

	TextureQualitySelectionBox->OnSelectionChanged.RemoveAll(this);
	TextureQualitySelectionBox->OnSelectionChanged.AddUObject(this, &UOptionWidget::OnTextureQualityChanged);

	ShadowQualitySelectionBox->OnSelectionChanged.RemoveAll(this);
	ShadowQualitySelectionBox->OnSelectionChanged.AddUObject(this, &UOptionWidget::OnShadowQualityChanged);

	ViewDistanceSelectionBox->OnSelectionChanged.RemoveAll(this);
	ViewDistanceSelectionBox->OnSelectionChanged.AddUObject(this, &UOptionWidget::OnViewDistanceChanged);

	AntiAliasingSelectionBox->OnSelectionChanged.RemoveAll(this);
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

	CamerSensitivityValueLabel->SetText(FText::AsNumber(int32(NewValue)));

	// NOTE: when changed in main menu, value is not set for ingame character so when actual player load re set the value on it.

	if (!PlayerController) return;
	// on 50 use default sens
	PlayerController->SetCameraSensitivity(NewValue/50);
}

void UOptionWidget::OnMusicVolumeChanged(float NewValue)
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Music Volume changed to %f"), NewValue);
	MusicValueLabel->SetText(FText::AsNumber(int32(NewValue)));

	UGameplayStatics::SetSoundMixClassOverride(
		this,
		SoundMixModifier,
		SoundClassForMusic,
		(NewValue / 100.f),
		1.f,
		0.1f
		);

	UGameplayStatics::PushSoundMixModifier(this, SoundMixModifier);
}

void UOptionWidget::OnSFXVolumeChanged(float NewValue)
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("SFX Volume changed to %f"), NewValue);
	SFXValueLabel->SetText(FText::AsNumber(int32(NewValue)));

	UGameplayStatics::SetSoundMixClassOverride(
		this,
		SoundMixModifier,
		SoundClassForSFX,
		(NewValue / 100.f),
		1.f,
		0.1f
		);

	UGameplayStatics::PushSoundMixModifier(this, SoundMixModifier);
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
