// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PauseMenu.h"
#include "Widget/OptionWidget.h"
#include "Widget/Components/Button/MasterButton.h"
#include "Macros/WidgetMacros.h"
#include "Utils/PropHuntLog.h"
#include "Controller/PropHuntPlayerController.h"

#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"

// NOTE: Handle pause menu creation after game end

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetIsFocusable(true);
	PlayerController = Cast<APropHuntPlayerController>(GetOwningPlayer());

	BindClickEvents();
}

void UPauseMenu::NativePreConstruct()
{
	Super::NativePreConstruct();

	ResumeButton->SetLabel("Resume");
	OptionButton->SetLabel("Options");
	QuitGameButton->SetLabel("Quit Game");
	BackButton->SetLabel("X");

	OptionWidget->SetVisibility(ESlateVisibility::Hidden);
	BackButton->SetVisibility(ESlateVisibility::Hidden);
}

void UPauseMenu::BindClickEvents()
{
	BIND_BUTTON_CLICK(ResumeButton, &UPauseMenu::OnResumeButtonClicked);
	BIND_BUTTON_CLICK(OptionButton, &UPauseMenu::OnOptionButtonClicked);
	BIND_BUTTON_CLICK(QuitGameButton, &UPauseMenu::OnQuitGameButtonClicked);
	BIND_BUTTON_CLICK(BackButton, &UPauseMenu::OnBackButtonClicked);
}

void UPauseMenu::OnResumeButtonClicked()
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Resume button clicked"));
	PlayerController->TogglePauseMenu();
}

void UPauseMenu::OnOptionButtonClicked()
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Option button clicked"));
	PauseMenuVBox->SetVisibility(ESlateVisibility::Hidden);
	OptionWidget->SetVisibility(ESlateVisibility::Visible);
	BackButton->SetVisibility(ESlateVisibility::Visible);
}

void UPauseMenu::OnQuitGameButtonClicked()
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Quit Game button clicked"));

	if (PlayerController)
	{
		if (PlayerController->GetIsHost())
		{
			PlayerController->ExitGame();
		}
		else
		{
			PlayerController->ExitClient();
		}

		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
	}
	else
	{
		UE_LOG_NON_SHIP(LogPropHuntWidget, Warning, TEXT("Failed to get game controller in OnExitGameButtonClicked"));
	}
}

void UPauseMenu::OnBackButtonClicked()
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Back button clicked"));
	PauseMenuVBox->SetVisibility(ESlateVisibility::Visible);
	OptionWidget->SetVisibility(ESlateVisibility::Hidden);
	BackButton->SetVisibility(ESlateVisibility::Hidden);
}

FReply UPauseMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();

	if (Key == EKeys::Escape || Key == EKeys::P)
	{
		if (!PlayerController) return FReply::Unhandled();

		PlayerController->TogglePauseMenu();

		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}
