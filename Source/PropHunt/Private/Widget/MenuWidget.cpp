// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MenuWidget.h"
#include "Widget/HostWidget.h"
#include "Widget/JoinGameWidget.h"
#include "Widget/Components/Button/MasterButton.h"
#include "Controller/MenuController.h"
#include "Utils/WidgetUtils.h"
#include "Utils/PropHuntLog.h"
#include "Utils/MapManager.h"
#include "Macros/WidgetMacros.h"
#include "Widget/UIManager.h"
//#include "GameInstance/PropHuntGameInstance.h"
//#include "Utils/MapManager.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/WidgetSwitcher.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Spacer.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Kismet/GameplayStatics.h"

void UMenuWidget::AddServerToList(UUserWidget* ServerEntry)
{
	JoinGameWidgetRef->AddServerToList(ServerEntry);
}

void UMenuWidget::ClearServerList()
{
	JoinGameWidgetRef->ClearServerList();
}

void UMenuWidget::DisplaySessionLoadingThrobber()
{
	JoinGameWidgetRef->DisplaySessionLoadingThrobber();
}

void UMenuWidget::HideSessionLoadingThrobber()
{
	JoinGameWidgetRef->HideSessionLoadingThrobber();
}

void UMenuWidget::DisplayNoSessionFoundMessage()
{
	JoinGameWidgetRef->DisplayNoSessionFoundMessage();
}

void UMenuWidget::HideNoSessionFoundMessage()
{
	JoinGameWidgetRef->HideNoSessionFoundMessage();
}

void UMenuWidget::SetBackgroundImage()
{
	//FMapInfo MapInfo = MapManager::GetAllMaps()[*(GetGameInstance<UPropHuntGameInstance>()->GetMapInfo().Name)];

	//FSlateBrush Brush;
	//Brush.SetResourceObject(MapInfo.Image);
	//Brush.ImageSize = FVector2D(MapInfo.Image->GetSizeX(), MapInfo.Image->GetSizeY());
	//BackgroundImage->SetBrush(Brush);
}

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MenuController = Cast<AMenuController>(GetOwningPlayer());
	BindClickEvents();

	SetBackgroundImage();
}

void UMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	InitializeComponents();
}

// Bind fucntion to click of buttons
void UMenuWidget::BindClickEvents()
{
	BIND_BUTTON_CLICK(PlayGameButton, &UMenuWidget::OnPlayGameButtonClicked);
	BIND_BUTTON_CLICK(OptionsButton, &UMenuWidget::OnOptionsButtonClicked);
	BIND_BUTTON_CLICK(QuitGameButton, &UMenuWidget::OnQuitGameButtonClicked);

	BIND_BUTTON_CLICK(HostGameButton, &UMenuWidget::OnHostGameButonClicked);
	BIND_BUTTON_CLICK(JoinGameButton, &UMenuWidget::OnJoinGameButtonClicked);

	BIND_BUTTON_CLICK(BackButton, &UMenuWidget::OnBackButtonClicked);

	ProfileButton->OnClicked.AddDynamic(this, &UMenuWidget::OnProfileButtonClicked);
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
	PlayGameMenuOverlay->SetVisibility(ESlateVisibility::Hidden);
	ProfileSectionOverlay->SetVisibility(ESlateVisibility::Visible);

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

	WidgetUtils::AddWidgetToWidgetSwitcher<UHostWidget>(this, PlayGameMenuSwitcher, UUIManager::Get()->HostWidgetBPClassRef);

	JoinGameWidgetRef = WidgetUtils::CreateAndValidateWidget<UJoinGameWidget>(this, UUIManager::Get()->JoinGameWidgetBPClassRef);
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
		BackButton->ButtonLabel->SetText(FText::FromString("X"));
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

void UMenuWidget::ChangeBackgroundTintToDark()
{
	BackgroundTint->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.5));
}

void UMenuWidget::ChangeBackgroundTintToLight()
{
	BackgroundTint->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.3));
}

void UMenuWidget::SwitchSessionButtonsProperty(bool HostButtonClicked)
{
	static FLinearColor BlueColor = FLinearColor::Blue;
	static FLinearColor GreyShade = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);

	HostGameButton->SetBackgroundColor(HostButtonClicked ? BlueColor : GreyShade);
	JoinGameButton->SetBackgroundColor(!HostButtonClicked ? BlueColor : GreyShade);
	PlayGameMenuSwitcher->SetActiveWidgetIndex(!HostButtonClicked);
}

// Delegate methods, bind with button click

void UMenuWidget::OnPlayGameButtonClicked()
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Play game button clicked!"));

	MenuState = EMenuState::PlayGameMenu;
	ChangeBackgroundTintToDark();
	MainMenuVBox->SetVisibility(ESlateVisibility::Hidden);
	ProfileButton->SetVisibility(ESlateVisibility::Hidden);
	PlayGameMenuOverlay->SetVisibility(ESlateVisibility::Visible);
	BackButton->SetVisibility(ESlateVisibility::Visible);
}

void UMenuWidget::OnOptionsButtonClicked()
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Options button clicked!"));
}

void UMenuWidget::OnQuitGameButtonClicked()
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Quit game button clicked!"));

	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UMenuWidget::OnHostGameButonClicked()
{
	SwitchSessionButtonsProperty(true);
}

void UMenuWidget::OnJoinGameButtonClicked()
{
	SwitchSessionButtonsProperty(false);
	MenuController->SearchSessions();
}

void UMenuWidget::OnBackButtonClicked()
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("Back button clicked!"));

	if (MenuState == EMenuState::PlayGameMenu)
	{
		MenuState = EMenuState::MainMenu;
		ChangeBackgroundTintToLight();
		PlayGameMenuOverlay->SetVisibility(ESlateVisibility::Hidden);
		// do all kind of cleanups
		MainMenuVBox->SetVisibility(ESlateVisibility::Visible);
		BackButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMenuWidget::OnProfileButtonClicked()
{
	UE_LOG_NON_SHIP(LogPropHuntWidget, Display, TEXT("profile button clicked!"));

	MenuState = EMenuState::ProfileMenu;

	ProfileSectionOverlay->SetVisibility(ESlateVisibility::Visible);
	BackButton->SetVisibility(ESlateVisibility::Visible);

	MainMenuVBox->SetVisibility(ESlateVisibility::Hidden);
	ProfileButton->SetVisibility(ESlateVisibility::Hidden);
}
