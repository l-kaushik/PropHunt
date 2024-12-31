// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MenuWidget.h"
#include "Widget/HostWidget.h"
#include "Widget/JoinGameWidget.h"
#include "Widget/Components/Button/MasterButton.h"
#include "Controller/MenuController.h"
#include "Utils/WidgetUtils.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/WidgetSwitcher.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Spacer.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"

// custom macro to make on click binding generic
#define BIND_BUTTON_CLICK(Button, Function) \
    if (Button)                             \
    {                                       \
        Button->OnClicked.AddUObject(this, Function); \
    }

UMenuWidget::UMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FString BasePath = "/Game/Widgets/";

	HostWidgetBPClassRef = AMenuController::LoadWidgetBlueprint<UHostWidget>(BasePath + FString("WB_Host"));
	JoinGameWidgetBPClassRef = AMenuController::LoadWidgetBlueprint<UJoinGameWidget>(BasePath + FString("WB_JoinGame"));
}

void UMenuWidget::AddServerToList(UUserWidget* ServerEntry)
{
	JoinGameWidgetRef->AddServerToList(ServerEntry);
}

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MenuController = Cast<AMenuController>(GetOwningPlayer());

	InitializeComponents();
	BindClickEvents();
}

void UMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	InitializeComponents();
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

// Initialize every component of the widget, by calling relevant function
void UMenuWidget::InitializeComponents()
{
	// Initialize MenuVBox components
	WidgetUtils::InitializeContainerBox(MainMenuVBox, PlayGameButton);
	WidgetUtils::InitializeContainerBox(MainMenuVBox, OptionsButton);
	WidgetUtils::InitializeContainerBox(MainMenuVBox, QuitGameButton);

	// Initialize SessionButton border components
	WidgetUtils::InitializeContainerBox<UBorder, UMasterButton>(JoinButtonBorder, JoinGameButton);
	WidgetUtils::InitializeContainerBox<UBorder, UMasterButton>(HostButtonBorder, HostGameButton);

	// Initialize SessionButtonHBox components
	WidgetUtils::InitializeContainerBox<UHorizontalBox, UBorder>(SessionButtonsHBox, JoinButtonBorder);
	WidgetUtils::InitializeContainerBox<UHorizontalBox, UBorder>(SessionButtonsHBox, HostButtonBorder);
	WidgetUtils::InitializeContainerBox(SessionButtonsHBox, SessionButtonSpacer);

	// Initialize PlayGameMenuVBox components
	WidgetUtils::InitializeContainerBox<UVerticalBox, UHorizontalBox>(PlayGameMenuVBox, SessionButtonsHBox);
	WidgetUtils::InitializeContainerBox<UVerticalBox, UWidgetSwitcher>(PlayGameMenuVBox, PlayGameMenuSwitcher);

	// Initialize all UMasterButtons
	InitializeMenuButton(PlayGameButton, "Play Game");
	InitializeMenuButton(OptionsButton, "Options");
	InitializeMenuButton(QuitGameButton, "Quit Game");
	InitializeMenuButton(HostGameButton, "Host Game");
	InitializeMenuButton(JoinGameButton, "Join Game");

	InitializeBackButton();
	FillPlayMenuWidgetSwitcher();
	SetupInitialProperties();
}

// Set initial values for components properties.

void UMenuWidget::SetupInitialProperties()
{
	PlayGameMenuVBox->SetVisibility(ESlateVisibility::Hidden);

	FLinearColor BroderBoxColor(0.5f, 0.5f, 0.5f, 0.1f);	// light grey

	JoinButtonBorder->SetBrushColor(BroderBoxColor);
	HostButtonBorder->SetBrushColor(BroderBoxColor);

	WidgetUtils::SetPaddingAndSize<UBorder, UHorizontalBoxSlot>(JoinButtonBorder, FMargin(0.0f), FSlateChildSize(ESlateSizeRule::Fill));
	WidgetUtils::SetPaddingAndSize<UBorder, UHorizontalBoxSlot>(HostButtonBorder, FMargin(0.0f), FSlateChildSize(ESlateSizeRule::Fill));
	WidgetUtils::SetPaddingAndSize<USpacer, UHorizontalBoxSlot>(SessionButtonSpacer, FMargin(0.0f), FSlateChildSize(ESlateSizeRule::Fill));
	WidgetUtils::SetPaddingAndSize<UWidgetSwitcher, UVerticalBoxSlot>(PlayGameMenuSwitcher, FMargin(0.0f), FSlateChildSize(ESlateSizeRule::Fill));

	PlayGameMenuSwitcher->SetActiveWidgetIndex(0);
	HostGameButton->Button->SetBackgroundColor(FLinearColor::Blue);
}

// Create, validate and then add widget in widget swticher

void UMenuWidget::FillPlayMenuWidgetSwitcher()
{
	PlayGameMenuSwitcher->ClearChildren();

	WidgetUtils::AddWidgetToWidgetSwitcher<UHostWidget>(this, PlayGameMenuSwitcher, HostWidgetBPClassRef);

	JoinGameWidgetRef = WidgetUtils::CreateAndValidateWidget<UJoinGameWidget>(this, JoinGameWidgetBPClassRef);
	PlayGameMenuSwitcher->AddChild(JoinGameWidgetRef);
	
}

// Main Menu UI Components
void UMenuWidget::InitializeMenuButton(UMasterButton* Button, FString ButtonLabel)
{
	if (Button)
	{
		Button->SetLabel(ButtonLabel);
		SetMainMenuButtons(Button);
	}
}

// Back button initializer

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

// Delegate methods, bind with button click

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
	HostGameButton->Button->SetBackgroundColor(FLinearColor::Blue);
	JoinGameButton->Button->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));
	PlayGameMenuSwitcher->SetActiveWidgetIndex(0);
}

void UMenuWidget::OnJoinGameButtonClicked()
{
	JoinGameButton->Button->SetBackgroundColor(FLinearColor::Blue);
	HostGameButton->Button->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));
	PlayGameMenuSwitcher->SetActiveWidgetIndex(1);
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
