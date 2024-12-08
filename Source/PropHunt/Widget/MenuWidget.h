// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Enumerator/PropHuntEnums.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
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
	void InitializePlayGameButton();
	void InitializeOptionsButton();
	void InitializeQuitGameButton();

	// Play Game Menu Components Initializers
	void InitializePlayGameMenuVBox();
	void InitializeHostGameButton();
	void InitializeJoinGameButton();

	void InitializeBackButton();

	// Utility functions
	void SetMainMenuButtons(class UButton* Button);

private:

	// Main Menu Components
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* MainMenuVBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* PlayGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OptionsButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitGameButton;


	// Play Game Menu Components
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* PlayGameMenuVBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinGameButton;

	// Utility components
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;

	EMenuState MenuState;
};
