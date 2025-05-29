// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MenuWidget.h"
#include "Widget/HostWidget.h"
#include "Widget/JoinGameWidget.h"
#include "Widget/Components/Button/MasterButton.h"
#include "Widget/MatchCardWidget.h"
#include "Controller/MenuController.h"
#include "Utils/WidgetUtils.h"
#include "Utils/PropHuntLog.h"
#include "Utils/MapManager.h"
#include "Macros/WidgetMacros.h"
#include "Widget/UIManager.h"
#include "Structs/PlayerData.h"
#include "Structs/ImageData.h"
#include "SaveGame/SaveGameManager.h"
#include "SaveGame/PropHuntSaveGame.h"
#include "States/PropHuntPlayerState.h"
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
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
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

void UMenuWidget::SetProfileData(FPlayerData InPlayerData)
{
	auto* Image = WidgetUtils::CreateTextureFromRawData(InPlayerData.ProfileImage);

	if (Image)
	{
		WidgetUtils::SetImageToButton(ChangeProfileImage, Image);
		WidgetUtils::SetImageToButton(ProfileButton, Image);
	}
	Username->SetText(FText::FromString(InPlayerData.Username));
	LastUsername = InPlayerData.Username;
}

void UMenuWidget::SetMatchHistoryData(FMatchHistoryMap InMatchHistoryData)
{
	MatchHistoryScrollBox->ClearChildren();

	if (InMatchHistoryData.IsEmpty())
	{
		MatchHistoryLabel->SetVisibility(ESlateVisibility::Visible);
		return;
	}

	MatchHistoryLabel->SetVisibility(ESlateVisibility::Hidden);

	// TODO: Consider using UListView with item pooling if match history grows significantly.

	MatchHistoryMap = InMatchHistoryData;
	LastMatchIndex = InMatchHistoryData.Keys.Num();
	// load data in scrollbox
	LoadMatchHistoryData();
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
	//ChangeProfileImage->OnClicked.AddDynamic(this, &UMenuWidget::OnChangeProfileImageClicked);
	ChangeProfileImage->SetIsEnabled(false); // disabled cuz of 64kb limit on profile picture replication
	Username->OnTextCommitted.AddDynamic(this, &UMenuWidget::OnUsernameCommitted);
	MatchHistoryScrollBox->OnUserScrolled.AddDynamic(this, &UMenuWidget::OnUserMatchHistoryScrolled);
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
	ProfileSectionOverlay->SetVisibility(ESlateVisibility::Hidden);

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

void UMenuWidget::OnBackButtonInPlayGameMenuClicked()
{
	ChangeBackgroundTintToLight();

	PlayGameMenuOverlay->SetVisibility(ESlateVisibility::Hidden);
	BackButton->SetVisibility(ESlateVisibility::Hidden);

	MainMenuVBox->SetVisibility(ESlateVisibility::Visible);
	ProfileButton->SetVisibility(ESlateVisibility::Visible);
}

void UMenuWidget::OnBackButtonInProfileMenuClicked()
{
	ProfileSectionOverlay->SetVisibility(ESlateVisibility::Hidden);
	BackButton->SetVisibility(ESlateVisibility::Hidden);
	MainMenuVBox->SetVisibility(ESlateVisibility::Visible);
	ProfileButton->SetVisibility(ESlateVisibility::Visible);

	// can show pop for confirming settings | directly save settings on change
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

	switch (MenuState)
	{
	case EMenuState::MainMenu:
		break;
	case EMenuState::PlayGameMenu:
		OnBackButtonInPlayGameMenuClicked();
		break;
	case EMenuState::OptionMenu:
		break;
	case EMenuState::ProfileMenu:
		OnBackButtonInProfileMenuClicked();
		break;
	default:
		break;
	}

	MenuState = EMenuState::MainMenu;
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

void UMenuWidget::OnChangeProfileImageClicked()
{
	// get image from dialog box and load on button

	auto* Image = WidgetUtils::OpenFileDialogueAndLoadImage();

	if (!Image) return;

	WidgetUtils::SetImageToButton(ChangeProfileImage, Image);
	WidgetUtils::SetImageToButton(ProfileButton, Image);

	// save to save game slot
	SaveImageData(Image);
}

void UMenuWidget::OnUsernameCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	APropHuntPlayerState* PlayerState = MenuController->GetPlayerState<APropHuntPlayerState>();
	if (!PlayerState) return;

	if (Text.EqualTo(FText::FromString(LastUsername))) return;

	UpdateOrLoadUsername();
	LastUsername = Text.ToString();
	PlayerState->SetUsername(LastUsername);
}

void UMenuWidget::OnUserMatchHistoryScrolled(float CurrentOffset)
{
	float MaxOffset = MatchHistoryScrollBox->GetScrollOffsetOfEnd();

	if ((MaxOffset - CurrentOffset) < 100.0f && (LastMatchIndex > 0))
	{
		LoadMatchHistoryData();
	}
}

void UMenuWidget::SaveImageData(UTexture2D* Image)
{
	APropHuntPlayerState* PlayerState = MenuController->GetPlayerState<APropHuntPlayerState>();
	FString username = Username->GetText().ToString();
	FImageData ImageData;
	if (!WidgetUtils::ExtractRawDataFromTexture(Image, ImageData)) return;
	auto* SaveGameInstance = SaveGameManager::Get().LoadGame(username);
	SaveGameInstance->PlayerData.ProfileImage = ImageData;
	SaveGameManager::Get().SaveGame(SaveGameInstance, username);
	PlayerState->SetProfileImage(ImageData);
}

void UMenuWidget::UpdateOrLoadUsername()
{
	FString username = Username->GetText().ToString();
	auto* SaveGameInstance = SaveGameManager::Get().LoadGame(username);
	SaveGameInstance->PlayerData.Username = username;
	SetProfileData(SaveGameInstance->PlayerData);
	SetMatchHistoryData(SaveGameInstance->MatchData);
	SaveGameManager::Get().SaveGame(SaveGameInstance, username);
}

void UMenuWidget::LoadMatchHistoryData()
{
	int count = 0;
	while ((count != 10) && (LastMatchIndex > 0))
	{
		LastMatchIndex--;

		auto* Match = MatchHistoryMap.GetByIndex(LastMatchIndex);
		if (!Match) continue;

		auto* MatchCardWidgetRef = WidgetUtils::CreateAndValidateWidget<UMatchCardWidget>(this, UUIManager::Get()->MatchCardWidgetBPClassRef);

		MatchCardWidgetRef->SetData(*Match);
		auto* AddedWidget = Cast<UScrollBoxSlot>(MatchHistoryScrollBox->AddChild(MatchCardWidgetRef));
		AddedWidget->SetPadding(FMargin(40.f, 20.f, 40.f, 20.f));

		count++;
	}
}
