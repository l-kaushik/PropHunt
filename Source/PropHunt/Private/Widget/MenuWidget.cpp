// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MenuWidget.h"
#include "Widget/HostWidget.h"
#include "Widget/JoinGameWidget.h"
#include "Widget/Components/Button/MasterButton.h"
#include "Controller/MenuController.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Kismet/GameplayStatics.h"

// custom macro to make on click binding generic
#define BIND_BUTTON_CLICK(Button, Function) \
    if (Button)                             \
    {                                       \
        Button->OnClicked.AddUObject(this, Function); \
    }

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeComponents();

	MenuController = Cast<AMenuController>(GetOwningPlayer());
}

void UMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	InitializeComponents();

	BindClickEvents();
}

// Bind fucntion to click of buttons
void UMenuWidget::BindClickEvents()
{
	UE_LOG(LogTemp, Warning, TEXT("Bind click events called"));

	BIND_BUTTON_CLICK(PlayGameButton, &UMenuWidget::OnPlayGameButtonClicked);
	BIND_BUTTON_CLICK(OptionsButton, &UMenuWidget::OnOptionsButtonClicked);
	BIND_BUTTON_CLICK(QuitGameButton, &UMenuWidget::OnQuitGameButtonClicked);

	BIND_BUTTON_CLICK(HostGameButton, &UMenuWidget::OnHostGameButonClicked);
	BIND_BUTTON_CLICK(JoinGameButton, &UMenuWidget::OnJoinGameButtonClicked);

	BIND_BUTTON_CLICK(BackButton, &UMenuWidget::OnBackButtonClicked);
}

void UMenuWidget::InitializeComponents()
{
	InitializeMainMenuVBox();
	InitializePlayGameMenuVBox();

	InitializeMenuButton(PlayGameButton, "Play Game");
	InitializeMenuButton(OptionsButton, "Options");
	InitializeMenuButton(QuitGameButton, "Quit Game");
	InitializeMenuButton(HostGameButton, "Host Game");
	InitializeMenuButton(JoinGameButton, "Join Game");

	InitializeBackButton();
}

// Functions to initialize components

void UMenuWidget::OnPlayGameButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Play Game Button Clicked!"));

	MenuState = EMenuState::PlayGameMenu;
	MainMenuVBox->SetVisibility(ESlateVisibility::Hidden);
	PlayGameMenuVBox->SetVisibility(ESlateVisibility::Visible);
	BackButton->SetVisibility(ESlateVisibility::Visible);
}

void UMenuWidget::OnOptionsButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Options Button Clicked!"));
}

void UMenuWidget::OnQuitGameButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Quit Game Button Clicked!"));
}

void UMenuWidget::OnHostGameButonClicked()
{
	MenuController->CreateHostWidget();
}

void UMenuWidget::OnJoinGameButtonClicked()
{;
	MenuController->CreateJoinWidget();
	MenuController->SearchSessions();
}

void UMenuWidget::OnBackButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Back Button Clicked!"));

	if (MenuState == EMenuState::PlayGameMenu)
	{
		MenuState = EMenuState::MainMenu;
		PlayGameMenuVBox->SetVisibility(ESlateVisibility::Hidden);
		// do all kind of cleanups
		MainMenuVBox->SetVisibility(ESlateVisibility::Visible);
		BackButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

// Main Menu UI Components

void UMenuWidget::InitializeMainMenuVBox()
{
	if (MainMenuVBox)
	{
		MainMenuVBox->AddChild(PlayGameButton);
		MainMenuVBox->AddChild(OptionsButton);
		MainMenuVBox->AddChild(QuitGameButton);
	}
}

void UMenuWidget::InitializeMenuButton(UMasterButton* Button, FString ButtonLabel)
{
	if (Button)
	{
		Button->ButtonLabel->SetText(FText::FromString(ButtonLabel));
		SetMainMenuButtons(Button);
	}
}

// Play Game Menu Components Initializers

void UMenuWidget::InitializePlayGameMenuVBox()
{
	if (PlayGameMenuVBox)
	{
		PlayGameMenuVBox->AddChildToVerticalBox(HostGameButton);
		PlayGameMenuVBox->AddChildToVerticalBox(JoinGameButton);

		PlayGameMenuVBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMenuWidget::InitializeBackButton()
{
	if (BackButton)
	{
		BackButton->ButtonLabel->SetText(FText::FromString("Back"));
		BackButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

// Utility functions

void UMenuWidget::SetMainMenuButtons(UMasterButton* Button)
{
	UVerticalBoxSlot* ButtonSlot = Cast<UVerticalBoxSlot>(Button->Slot);
	if (ButtonSlot)
	{
		ButtonSlot->SetPadding(FMargin(0.0f, 10.0f, 0.0f, 10.0f));
		ButtonSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}
}
