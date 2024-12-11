// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHud.generated.h"

/**
 * 
 */
UCLASS()
class PROPHUNT_API UMainHud : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupPropWidget(bool bIsProp);
	void UpdateHealthBar(float NewHealth);
	void PlayHitMarkerAnimation();
	void ShowWinScreen(bool bIsPropWon, bool bIsProp);
	void StartTimer();
	void EndTimer();
	void UpdateTimerText();

protected:

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

private:
	void InitializeWidgetComponents();
	void SetGameStatusText();
	void SetCrosshairImage();
	void SetHitMarker();
	void SetHealthBar();
	void SetWinScreen();
	void SetWinScreenText();
	void SetNewGameStartingText();
	void SetTimerBorder();
	void SetTimerIcon();
	void SetTimerText();
	void HitMarkerAnimFinished(UWidgetAnimation* Animation);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GameStatus;

	UPROPERTY(meta = (BindWidget))
	class UImage* Crosshair;

	UPROPERTY(meta = (BindWidget))
	class UImage* HitMarker;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UBackgroundBlur* WinScreen;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WinScreenText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NewGameStartingText;

	UPROPERTY(meta = (BindWidget))
	class UBorder* TimerBorder;

	UPROPERTY(meta = (BindWidget))
	class UImage* TimerIcon;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimerText;

	FTimerHandle CountdownTimer;

	UPROPERTY(Transient,meta = (BindWidgetAnim))
	UWidgetAnimation* HitMarkerAnim;
};
