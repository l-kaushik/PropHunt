// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionWidget.generated.h"

class UMasterButton;
class UWidgetSwitcher;
class USlider;

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

	UFUNCTION()
	void OnGameplayButtonClicked();

	UFUNCTION()
	void OnAudioButtonClicked();

	UFUNCTION()
	void OnVideoButtonClicked();

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



	// Audio Settings



	// Video Settings
	
};
