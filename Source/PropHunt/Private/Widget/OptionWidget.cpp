// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/OptionWidget.h"
#include "Widget/Components/Button/MasterButton.h"
#include "Widget/Components/SelectionBox.h"
#include "Macros/WidgetMacros.h"
#include "Utils/WidgetUtils.h"
#include "Utils/PropHuntLog.h"
#include "SaveGame/SaveGameManager.h"
#include "SaveGame/PropHuntSaveGame.h"
#include "Controller/PropHuntPlayerController.h"

#include "Components/WidgetSwitcher.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"

#define APPLY_GAME_SETTING_DEFAULT(UserObject, SetFunctionName) \
do { \
	auto* GameSettings = UGameUserSettings::GetGameUserSettings(); \
	if (GameSettings) { \
		if (OverallGraphicsSelectionBox->GetSelectedOptionIndex() != 0) { \
			OverallGraphicsSelectionBox->SetActiveOption(0); \
		} \
		GameSettings->SetFunctionName(UserObject->GetSelectedOptionIndex()); \
		GameSettings->ApplySettings(false); \
		GameSettings->SaveSettings(); \
	} \
} while(0)

#define SAVE_GAME_SETTING(MemberName, ObjectValue) \
do { \
	SaveGameManager SGM_Instance = SaveGameManager::Get(); \
	auto& SlotName = SGM_Instance.GetLastSaveGameSlotName(); \
	auto* SaveGameInstance = SGM_Instance.LoadGame(SlotName); \
	SaveGameInstance->Settings.MemberName = ObjectValue; \
	SGM_Instance.SaveGame(SaveGameInstance, SlotName); \
} while(0)


void UOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// load controller
	PlayerController = GetOwningPlayer<APropHuntPlayerController>();

	BindClickEvent();
	BindSliderEvents();
	BindSelectionBoxEvents();

	// load values
	LoadGameSettings();
}

void UOptionWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	InitializeLabels();
	InitializeSliders();
}

void UOptionWidget::LoadGameSettings()
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Loading game settings..."));

	// load required settings from save game
	SaveGameManager SGM_Instance = SaveGameManager::Get();
	auto* SaveGameInstance = SGM_Instance.LoadGame(SGM_Instance.GetLastSaveGameSlotName());
	FSettingsData Settings = SaveGameInstance->Settings;

	// that means running first time, detect and apply settings
	if (Settings.OverallGraphics.IsEmpty())
	{
		ApplyHardwareDetectedGraphics();
	}
	else
	{
		CameraSensitivitySlider->SetValue(Settings.CameraSensitivity);
		MusicVolumeSlider->SetValue(Settings.MusicVolume);
		SFXVolumeSlider->SetValue(Settings.SFXVolume);
		ScreenPercentageSlider->SetValue(Settings.ScreenPercentage);
		UpdateOtherSettings();
		OverallGraphicsSelectionBox->SetActiveOptionTextOnly(Settings.OverallGraphics);
		AntiAliasingMethodSelectionBox->SetActiveOptionTextOnly(Settings.AntiAliasingMethod);
	}
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

	ScreenPercentageSlider->OnValueChanged.RemoveDynamic(this, &UOptionWidget::OnScreenPercentageValueChanged);
	ScreenPercentageSlider->OnValueChanged.AddDynamic(this, &UOptionWidget::OnScreenPercentageValueChanged);

	ScreenPercentageSlider->OnMouseCaptureEnd.RemoveDynamic(this, &UOptionWidget::OnScreenPercentageSliderMouseEnd);
	ScreenPercentageSlider->OnMouseCaptureEnd.AddDynamic(this, &UOptionWidget::OnScreenPercentageSliderMouseEnd);
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

	AntiAliasingMethodSelectionBox->OnSelectionChanged.RemoveAll(this);
	AntiAliasingMethodSelectionBox->OnSelectionChanged.AddUObject(this, &UOptionWidget::OnAntiAliasingMethodChanged);
}

void UOptionWidget::ApplyOtherSettings()
{
	APPLY_GAME_SETTING_DEFAULT(TextureQualitySelectionBox, SetTextureQuality);
	APPLY_GAME_SETTING_DEFAULT(ShadowQualitySelectionBox, SetShadowQuality);
	APPLY_GAME_SETTING_DEFAULT(ViewDistanceSelectionBox, SetViewDistanceQuality);
	APPLY_GAME_SETTING_DEFAULT(AntiAliasingSelectionBox, SetAntiAliasingQuality);
}

void UOptionWidget::UpdateOtherSettings()
{
	auto* GameSettings = UGameUserSettings::GetGameUserSettings();
	if (!GameSettings) return;

	TextureQualitySelectionBox->SetActiveOptionTextOnly(GameSettings->GetTextureQuality());
	ShadowQualitySelectionBox->SetActiveOptionTextOnly(GameSettings->GetShadowQuality());
	ViewDistanceSelectionBox->SetActiveOptionTextOnly(GameSettings->GetViewDistanceQuality());
	AntiAliasingSelectionBox->SetActiveOptionTextOnly(GameSettings->GetAntiAliasingQuality());
}

void UOptionWidget::ApplyHardwareDetectedGraphics()
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Detecting and applying suitable graphic settings..."));

	auto* GameSettings = UGameUserSettings::GetGameUserSettings();
	GameSettings->RunHardwareBenchmark();
	GameSettings->ApplyHardwareBenchmarkResults();

	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Scalability level: %d"), GameSettings->GetOverallScalabilityLevel());
	UpdateOtherSettings();
	OverallGraphicsSelectionBox->SetActiveOption(0);
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

	SAVE_GAME_SETTING(CameraSensitivity, NewValue);
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
	SAVE_GAME_SETTING(MusicVolume, NewValue);
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
	SAVE_GAME_SETTING(SFXVolume, NewValue);
}

void UOptionWidget::OnScreenPercentageValueChanged(float NewValue)
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("ScreenPercentage changed to %f"), NewValue);

	ScreenPercentagValueLabel->SetText(FText::AsNumber(int32(NewValue)));
}

void UOptionWidget::OnScreenPercentageSliderMouseEnd()
{
	int32 ScreenPercent = FMath::RoundToInt32(ScreenPercentageSlider->GetValue());
	FString Command = FString::Printf(TEXT("r.ScreenPercentage %d"), ScreenPercent);

	GetWorld()->GetFirstPlayerController()->ConsoleCommand(*Command);

	SAVE_GAME_SETTING(ScreenPercentage, ScreenPercent);
}

void UOptionWidget::OnOverallGraphicsChanged(const FString& NewOption)
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Overall Graphics changed to %s"), *NewOption);

	auto* GameSettings = UGameUserSettings::GetGameUserSettings();

	GameSettings->SetOverallScalabilityLevel(OverallGraphicsSelectionBox->GetSelectedOptionIndex() - 1);

	GameSettings->ApplySettings(false);

	// on custom, set apply other settings

	if (OverallGraphicsSelectionBox->GetSelectedOptionIndex() == 0)
	{
		ApplyOtherSettings();
	}
	else
	{
		UpdateOtherSettings();
	}

	SAVE_GAME_SETTING(OverallGraphics, OverallGraphicsSelectionBox->GetSelectedOptionString());
}

void UOptionWidget::OnTextureQualityChanged(const FString& NewOption)
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Texture Quality changed to %s"), *NewOption);

	APPLY_GAME_SETTING_DEFAULT(TextureQualitySelectionBox, SetTextureQuality);
}

void UOptionWidget::OnShadowQualityChanged(const FString& NewOption)
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Shadow Quality changed to %s"), *NewOption);

	APPLY_GAME_SETTING_DEFAULT(ShadowQualitySelectionBox, SetShadowQuality);
}

void UOptionWidget::OnViewDistanceChanged(const FString& NewOption)
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("View Distance changed to %s"), *NewOption);

	APPLY_GAME_SETTING_DEFAULT(ViewDistanceSelectionBox, SetViewDistanceQuality);
} 

void UOptionWidget::OnAntiAliasingChanged(const FString& NewOption)
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Anti Aliasing changed to %s"), *NewOption);
	
	APPLY_GAME_SETTING_DEFAULT(AntiAliasingSelectionBox, SetAntiAliasingQuality);
}

void UOptionWidget::OnAntiAliasingMethodChanged(const FString& NewOption)
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Anti Aliasing method changed to %s"), *NewOption);

	int32 AAMIndex = AntiAliasingMethodSelectionBox->GetSelectedOptionIndex();
	FString Command = FString::Printf(TEXT("r.AntiAliasingMethod %d"), AAMIndex);

	GetWorld()->GetFirstPlayerController()->ConsoleCommand(*Command);

	SAVE_GAME_SETTING(AntiAliasingMethod, AAMIndex);
}
