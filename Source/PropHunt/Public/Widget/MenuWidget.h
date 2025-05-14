// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enumerator/PropHuntEnums.h"
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
class UButton;
class UEditableText;
class UScrollBox;
class UTextBlock;

struct FPlayerData;
struct FMatchHistoryMap;

UCLASS()
class PROPHUNT_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void AddServerToList(UUserWidget* ServerEntry);
	void ClearServerList();
	void DisplaySessionLoadingThrobber();
	void HideSessionLoadingThrobber();
	void DisplayNoSessionFoundMessage();
	void HideNoSessionFoundMessage();
	void SetBackgroundImage();

	void SetProfileData(FPlayerData InPlayerData);
	void SetMatchHistoryData(FMatchHistoryMap InMatchHistoryData);

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
	void OnBackButtonInPlayGameMenuClicked();
	void OnBackButtonInProfileMenuClicked();

	void SaveProfileData();

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

	UFUNCTION()
	void OnProfileButtonClicked();

	UFUNCTION()
	void OnChangeProfileImageClicked();

	UFUNCTION()
	void OnUsernameCommitted(const FText& Text, ETextCommit::Type CommitMethod);

private:
	// instance variables
	EMenuState MenuState;
	AMenuController* MenuController;
	UJoinGameWidget* JoinGameWidgetRef;

	// Main Menu Components
	UPROPERTY(meta = (BindWidget))
	UImage* BackgroundTint;

	UPROPERTY(meta = (BindWidget))
	UImage* BackgroundImage;

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

	// profile section
	UPROPERTY(meta = (BindWidget))
	UButton* ProfileButton;

	UPROPERTY(meta = (BindWidget))
	UOverlay* ProfileSectionOverlay;

	UPROPERTY(meta = (BindWidget))
	UButton* ChangeProfileImage;

	UPROPERTY(meta = (BindWidget))
	UEditableText* Username;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* MatchHistoryScrollBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MatchHistoryLabel;

	// Utility components
	UPROPERTY(meta = (BindWidget))
	UMasterButton* BackButton;	// represented as close icon in game

	FString LastUsername;
};
