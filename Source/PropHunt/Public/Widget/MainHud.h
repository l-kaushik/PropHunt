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
class UOverlay;
class UTextBlock;
class UProgressBar;
class UMasterButton;
class UHorizontalBox;
class UWidgetSwitcher;
class UBackgroundBlur;
class UScoreboardMenuWidget;

class APropHuntGameState;

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
	void UpdateWeaponUI(int32 InCurrentAmmoInMagazine, int32 InCurrentReserverAmmo);

protected:

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

private:
	void BindButtonClicks();
	void InitializeWidgetComponents();
	void SetMasterButtonLabel(UMasterButton* Button,const FString& ButtonLabel);
	void SetPlayerObjectiveText();
	void SetCrosshairImage();
	void SetHitMarker();
	void SetHealthBar();
	void SetWinScreen();
	void SetWinScreenText();
	void SetTimerBorder();
	void SetTimerIcon();
	void SetTimerText();
	void SetWeaponUIElement();
	void HitMarkerAnimFinished(UWidgetAnimation* Animation);
	void HideHudComponents();
	void HideWeaponUIComponents(ESlateVisibility InVisibility);
	
	void OnNewGameButtonClicked();
	void OnExitGameButtonClicked();

	void FillScoreboardData();
	void LoadDataForGameStatsList(APropHuntGameState* GameState);
	void LoadDataForTopPerformerWindow();

	// HUD elements
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerObjective;

	UPROPERTY(meta = (BindWidget))
	UImage* Crosshair;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	// weapon UI

	UPROPERTY(meta = (BindWidget))
	UBorder* BulletCountBorder;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* BulletCountHBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentAmmoCount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Divider;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ReserveAmmoCount;

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

	// scoreboard overlay
	UPROPERTY(meta = (BindWidget))
	UOverlay* ScoreboardOverlay;

	// scoreboard menu 
	UPROPERTY(meta = (BindWidget))
	UScoreboardMenuWidget* ScoreboardMenu;

	// controls HBox buttons
	UPROPERTY(meta = (BindWidget))
	UMasterButton* NewGameButton;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* ExitGameButton;
	
};
