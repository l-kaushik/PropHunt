// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

class UMasterButton;
class UVerticalBox;
class UOptionWidget;
class UHelpWidget;

UCLASS()
class PROPHUNT_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

protected:

	void BindClickEvents();

	UFUNCTION()
	void OnResumeButtonClicked();

	UFUNCTION()
	void OnOptionButtonClicked();

	UFUNCTION()
	void OnHowToPlayButtonClicked();

	UFUNCTION()
	void OnQuitGameButtonClicked();

	UFUNCTION()
	void OnBackButtonClicked();

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

private:
	class APropHuntPlayerController* PlayerController;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PauseMenuVBox;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* OptionButton;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* HowToPlayButton;

	UPROPERTY(meta = (BindWidget))
	UOptionWidget* OptionWidget;

	UPROPERTY(meta = (BindWidget))
	UHelpWidget* HelpWidget;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* QuitGameButton;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* BackButton;
};
