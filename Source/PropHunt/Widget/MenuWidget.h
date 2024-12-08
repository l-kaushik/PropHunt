// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	// Functions to initialize components
	void InitializeMainMenuVBox();
	void InitializePlayGameButton();
	void InitializeOptionsButton();
	void InitializeQuitGameButton();

	// Formatting
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

};
