// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Enumerator/PropHuntEnums.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */

class UMasterButton;
class UWidgetSwitcher;
class UVerticalBox;
class AMenuController;
class UHorizontalBox;
class USpacer;
class UHostWidget;
class UJoinGameWidget;
class UBorder;
class UImage;
class UOverlay;

UCLASS()
class PROPHUNT_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void AddServerToList(UUserWidget* ServerEntry);
	void ClearServerList();
	void DisplaySessionLoadingThrobber();
	void HideSessionLoadingThrobber();

private:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	void BindClickEvents();

	void InitializeComponents();
	void SetupInitialProperties();
	void FillPlayMenuWidgetSwitcher();
	void InitializeMenuButton(UMasterButton* Button, FString ButtonLabel);
	void InitializeBackButton();
	void SetMainMenuButtons(UMasterButton* Button);
	void ChangeBackgroundTintToDark();
	void ChangeBackgroundTintToLight();
	void SwitchSessionButtonsProperty(bool HostButtonClicked = true);

	// OnClicked event functions
	UFUNCTION()
	void OnPlayGameButtonClicked();
	
	UFUNCTION()
	void OnOptionsButtonClicked();

	UFUNCTION()
	void OnQuitGameButtonClicked();

	UFUNCTION()
	void OnHostGameButonClicked();

	UFUNCTION()
	void OnJoinGameButtonClicked();

	UFUNCTION()
	void OnBackButtonClicked();

private:
	// instance variables
	EMenuState MenuState;
	AMenuController* MenuController;
	UJoinGameWidget* JoinGameWidgetRef;

	// Main Menu Components
	UPROPERTY(meta = (BindWidget))
	UImage* BackgroundTint;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* MainMenuVBox;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* PlayGameButton;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* OptionsButton;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* QuitGameButton;

	// Play Game Menu Components
	UPROPERTY(meta = (BindWidget))
	UOverlay* PlayGameMenuOverlay;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PlayGameMenuVBox;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* SessionButtonsHBox;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* PlayGameMenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* HostGameButton;

	UPROPERTY(meta = (BindWidget))
	UBorder* HostButtonBorder;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* JoinGameButton;

	UPROPERTY(meta = (BindWidget))
	UBorder* JoinButtonBorder;

	UPROPERTY(meta = (BindWidget))
	USpacer* SessionButtonSpacer;

	// Utility components
	UPROPERTY(meta = (BindWidget))
	UMasterButton* BackButton;	// represented as close icon in game
};
