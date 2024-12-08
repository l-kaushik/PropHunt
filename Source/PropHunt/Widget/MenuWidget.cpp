// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "../Widget/HostWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"


UMenuWidget::UMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UHostWidget> HostWidgetBPClass(TEXT("/Game/ThirdPerson/Widgets/WB_Host"));
	if (HostWidgetBPClass.Succeeded())
	{
		HostWidgetBPClassRef = HostWidgetBPClass.Class;
	}
}

void UMenuWidget::NativeConstruct()
{
	BindClickEvents();
	InitializeComponents();
}

void UMenuWidget::NativePreConstruct()
{
	InitializeComponents();
}

// Bind fucntion to click of buttons
void UMenuWidget::BindClickEvents()
{
	if (PlayGameButton)
	{
		PlayGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnPlayGameButtonClicked);
	}
	if (HostGameButton)
	{
		HostGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnHostGameButonClicked);
	}
	if (JoinGameButton)
	{
		JoinGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnJoinGameButtonClicked);
	}
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UMenuWidget::OnBackButtonClicked);
	}
}

void UMenuWidget::InitializeComponents()
{
	InitializeMainMenuVBox();
	InitializePlayGameButton();
	InitializeOptionsButton();
	InitializeQuitGameButton();

	InitializePlayGameMenuVBox();
	InitializeHostGameButton();
	InitializeJoinGameButton();

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
	UE_LOG(LogTemp, Warning, TEXT("Host Game Button Clicked!"));

	if (!HostWidgetBPClassRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Host widget blueprint class reference is nullptr"));
		return;
	}

	this->SetVisibility(ESlateVisibility::Collapsed);
	CreateAndAddWidget<UHostWidget>(HostWidgetBPClassRef);

}

void UMenuWidget::OnJoinGameButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Join Game Button Clicked!"));
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

void UMenuWidget::InitializeHostGameButton()
{
	if (HostGameButton)
	{
		SetMainMenuButtons(HostGameButton);
	}
}

void UMenuWidget::InitializeJoinGameButton()
{
	if (JoinGameButton)
	{
		SetMainMenuButtons(JoinGameButton);
	}
}

void UMenuWidget::InitializeBackButton()
{
	if (BackButton)
	{
		BackButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

// Utility functions

void UMenuWidget::SetMainMenuButtons(UButton* Button)
{
	UVerticalBoxSlot* ButtonSlot = Cast<UVerticalBoxSlot>(Button->Slot);
	if (ButtonSlot)
	{
		ButtonSlot->SetPadding(FMargin(0.0f, 10.0f, 0.0f, 10.0f));
		ButtonSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}
}
