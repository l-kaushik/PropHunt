// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"


void UMenuWidget::NativeConstruct()
{
	BindClickEvents();
	InitializeComponents();
}

void UMenuWidget::NativePreConstruct()
{
	InitializeComponents();
}

// Delegate binding
void UMenuWidget::BindClickEvents()
{
	if (PlayGameButton)
	{
		PlayGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnPlayGameButtonClicked);
	}

	if (OptionsButton)
	{
		OptionsButton->OnClicked.AddDynamic(this, &UMenuWidget::OnOptionsButtonClicked);
	}

	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnQuitGameButtonClicked);
	}
}

void UMenuWidget::InitializeComponents()
{
	InitializeMainMenuVBox();
	InitializePlayGameButton();
	InitializeOptionsButton();
	InitializeQuitGameButton();
}

// Functions to initialize components

void UMenuWidget::OnPlayGameButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Play Game Button Clicked!"));
}

void UMenuWidget::OnOptionsButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Options Button Clicked!"));
}

void UMenuWidget::OnQuitGameButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Quit Game Button Clicked!"));
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

void UMenuWidget::InitializePlayGameButton()
{
	if (PlayGameButton)
	{
		SetMainMenuButtons(PlayGameButton);
	}
}

void UMenuWidget::InitializeOptionsButton()
{
	if (OptionsButton)
	{
		SetMainMenuButtons(OptionsButton);
	}
}

void UMenuWidget::InitializeQuitGameButton()
{
	if (QuitGameButton)
	{
		SetMainMenuButtons(QuitGameButton);

	}
}

void UMenuWidget::SetMainMenuButtons(UButton* Button)
{
	UVerticalBoxSlot* ButtonSlot = Cast<UVerticalBoxSlot>(Button->Slot);
	if (ButtonSlot)
	{
		ButtonSlot->SetPadding(FMargin(0.0f, 10.0f, 0.0f, 10.0f));
		ButtonSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}
}
