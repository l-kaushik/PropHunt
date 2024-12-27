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
UCLASS()
class PROPHUNT_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	void BindClickEvents();
	void InitializeComponents();

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

	// Functions to initialize components
	// Main Menu Components Initializers
	void InitializeMainMenuVBox();
	void InitializeMenuButton(UMasterButton* Button, FString ButtonLabel);

	// Play Game Menu Components Initializers
	void InitializePlayGameMenuVBox();
	void InitializeHostGameButton();
	void InitializeJoinGameButton();

	void InitializeBackButton();

	// Utility functions
	void SetMainMenuButtons(UMasterButton* Button);

	// sub widget creator for this Menu Widget

private:

	// Main Menu Components
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* MainMenuVBox;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* PlayGameButton;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* OptionsButton;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* QuitGameButton;


	// Play Game Menu Components
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* PlayGameMenuVBox;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* HostGameButton;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* JoinGameButton;

	// Utility components
	UPROPERTY(meta = (BindWidget))
	UMasterButton* BackButton;

	EMenuState MenuState;
	class AMenuController* MenuController;
};
