// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sound/SoundMix.h"


#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionWidget.generated.h"

class UMasterButton;
class UWidgetSwitcher;
class USlider;
class USelectionBox;
class UTextBlock;

UCLASS()
class PROPHUNT_API UOptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	void LoadGameSettings();

protected:
	void InitializeLabels();
	void InitializeSliders();
	void BindClickEvent();
	void BindSliderEvents();
	void BindSelectionBoxEvents();
	void ApplyOtherSettings();
	void UpdateOtherSettings();
	void ApplyHardwareDetectedGraphics();

protected:

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

	UFUNCTION()
	void OnScreenPercentageValueChanged(float NewValue);

	UFUNCTION()
	void OnScreenPercentageSliderMouseEnd();

	void OnOverallGraphicsChanged(const FString& NewOption);
	void OnTextureQualityChanged(const FString& NewOption);
	void OnShadowQualityChanged(const FString& NewOption);
	void OnViewDistanceChanged(const FString& NewOption);
	void OnAntiAliasingChanged(const FString& NewOption);
	void OnAntiAliasingMethodChanged(const FString& NewOption);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundMix* SoundMixModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundClass* SoundClassForMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundClass* SoundClassForSFX;

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

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CamerSensitivityValueLabel;

	// Audio Settings

	UPROPERTY(meta = (BindWidget))
	USlider* MusicVolumeSlider;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MusicValueLabel;

	UPROPERTY(meta = (BindWidget))
	USlider* SFXVolumeSlider;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SFXValueLabel;

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

	UPROPERTY(meta = (BindWidget))
	USelectionBox* AntiAliasingMethodSelectionBox;

	UPROPERTY(meta = (BindWidget))
	USlider* ScreenPercentageSlider;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScreenPercentagValueLabel;

	// utility
	class APropHuntPlayerController* PlayerController;
};
