// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionWidget.generated.h"

class UMasterButton;
class UWidgetSwitcher;
class USlider;
class USelectionBox;

UCLASS()
class PROPHUNT_API UOptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

protected:
	void InitializeLabels();
	void BindClickEvent();
	void BindSliderEvents();
	void BindSelectionBoxEvents();

	UFUNCTION()
	void OnGameplayButtonClicked();

	UFUNCTION()
	void OnAudioButtonClicked();

	UFUNCTION()
	void OnVideoButtonClicked();

	UFUNCTION()
	void OnCameraSensitivityChanged(float NewValue);

	UFUNCTION()
	void OnMusicVolumeChanged(float NewValue);

	UFUNCTION()
	void OnSFXVolumeChanged(float NewValue);

	void OnOverallGraphicsChanged(const FString& NewOption);
	void OnTextureQualityChanged(const FString& NewOption);
	void OnShadowQualityChanged(const FString& NewOption);
	void OnViewDistanceChanged(const FString& NewOption);
	void OnAntiAliasingChanged(const FString& NewOption);

private:

	// menu switcher buttons

	UPROPERTY(meta = (BindWidget))
	UMasterButton* GameplayButton;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* AudioButton;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* VideoButton;

	// Menu Switcher
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	// Gameplay settings

	UPROPERTY(meta = (BindWidget))
	USlider* CameraSensitivitySlider;

	// Audio Settings

	UPROPERTY(meta = (BindWidget))
	USlider* MusicVolumeSlider;

	UPROPERTY(meta = (BindWidget))
	USlider* SFXVolumeSlider;

	// Video Settings

	UPROPERTY(meta = (BindWidget))
	USelectionBox* OverallGraphicsSelectionBox;

	UPROPERTY(meta = (BindWidget))
	USelectionBox* TextureQualitySelectionBox;

	UPROPERTY(meta = (BindWidget))
	USelectionBox* ShadowQualitySelectionBox;

	UPROPERTY(meta = (BindWidget))
	USelectionBox* ViewDistanceSelectionBox;

	UPROPERTY(meta = (BindWidget))
	USelectionBox* AntiAliasingSelectionBox;

};
