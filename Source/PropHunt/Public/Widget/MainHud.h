// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHud.generated.h"

/**
 * 
 */

class UImage;
class UBorder;
class UTextBlock;
class UProgressBar;
class UBackgroundBlur;

UCLASS()
class PROPHUNT_API UMainHud : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupPropWidget(bool bIsProp);
	void UpdateHealthBar(float NewHealth);
	void PlayHitMarkerAnimation();
	void ShowWinScreen(bool bIsHost);
	void StartTimer();
	void EndTimer();
	void UpdateTimerText();

protected:

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

private:
	void InitializeWidgetComponents();
	void SetPlayerObjectiveText();
	void SetCrosshairImage();
	void SetHitMarker();
	void SetHealthBar();
	void SetWinScreen();
	void SetWinScreenText();
	void SetTimerBorder();
	void SetTimerIcon();
	void SetTimerText();
	void HitMarkerAnimFinished(UWidgetAnimation* Animation);


	// HUD elements
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerObjective;

	UPROPERTY(meta = (BindWidget))
	UImage* Crosshair;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	// Hit marker and animation

	UPROPERTY(meta = (BindWidget))
	UImage* HitMarker;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* HitMarkerAnim;

	// Win screen

	UPROPERTY(meta = (BindWidget))
	UBackgroundBlur* WinScreen;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WinScreenText;

	// Timer

	UPROPERTY(meta = (BindWidget))
	UBorder* TimerBorder;

	UPROPERTY(meta = (BindWidget))
	UImage* TimerIcon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimerText;

	FTimerHandle CountdownTimer;
};
