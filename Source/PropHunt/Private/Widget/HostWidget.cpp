// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HostWidget.h"
#include "Widget/MenuWidget.h"
#include "Widget/UIManager.h"
#include "Widget/ErrorBox/UIErrorBox.h"
#include "Controller/MenuController.h"
#include "Utils/PropHuntLog.h"
#include "Utils/GlobalUtils.h"
#include "Utils/WidgetUtils.h"
#include "Utils/Struct.h"
#include "Utils/MapManager.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Widget/Components/Button/MasterButton.h"
#include "Internationalization/Regex.h"

UHostWidget::UHostWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	m_MapSelected = 0;

	// fill up MapInfoArray
	for (const auto& Pair : MapManager::GetAllMaps())
	{
		if (Pair.Key != "MenuMap")
		{
			m_MapInfoArray.Push(Pair.Value);
		}
	}
}

void UHostWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BindEvents();
	UpdateMapData();

	MenuController = Cast<AMenuController>(GetOwningPlayer());
}

void UHostWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	InitializeHostButton();
}

void UHostWidget::BindEvents()
{
	if (HostButton)
	{
		HostButton->OnClicked.RemoveAll(this);
		HostButton->OnClicked.AddUObject(this, &UHostWidget::OnHostButtonClicked);
	}

	LeftImageSelector->OnClicked.AddDynamic(this, &UHostWidget::OnLeftImageSelectorClicked);
	RightImageSelector->OnClicked.AddDynamic(this, &UHostWidget::OnRightImageSelectorClicked);
}

void UHostWidget::InitializeHostButton()
{
	HostButton->SetLabel("Host");
}

void UHostWidget::OnHostButtonClicked()
{
	if (!VerifyServerInfo()) return;

	FName SessionName(*ServerNameText->GetText().ToString());
	int32 PlayerNumbers = FCString::Atoi(*(NumberOfPlayersText->GetText().ToString()));
	
	// use controller to host server
	MenuController->ClientWantsToHost(SessionName, m_MapInfoArray[m_MapSelected], PlayerNumbers);

}

void UHostWidget::OnLeftImageSelectorClicked()
{
	m_MapSelected = (m_MapSelected - 1 + m_MapInfoArray.Num()) % m_MapInfoArray.Num();
	UpdateMapData();
}

void UHostWidget::OnRightImageSelectorClicked()
{
	m_MapSelected = ++m_MapSelected % m_MapInfoArray.Num();
	UpdateMapData();
}

void UHostWidget::UpdateMapData()
{
	FMapInfo MapInfo = m_MapInfoArray[m_MapSelected];
	FString TrimmedMapName = MapInfo.Name.LeftChop(3);
	FSlateBrush Brush;
	Brush.SetResourceObject(MapInfo.Image);
	Brush.ImageSize = FVector2D(MapInfo.Image->GetSizeX(), MapInfo.Image->GetSizeY());

	MapName->SetText(FText::FromString(TrimmedMapName));
	MapImage->SetBrush(Brush);
}

bool UHostWidget::VerifyServerInfo()
{
	const FString ServerName = ServerNameText->GetText().ToString();
	const FString PlayerNumber = NumberOfPlayersText->GetText().ToString();

	/*
	*	Verify server name
	* 
	*	Length should be between 3 - 20 chars
	*	Only start with alphabet
	*	Can contain numbers and _
	*/
	const FRegexPattern ServerNamePattern(TEXT("^[a-zA-Z][a-zA-Z0-9_]{2,19}$"));
	FRegexMatcher ServerNameMatcher(ServerNamePattern, ServerName);

	if (!ServerNameMatcher.FindNext()) {
		ShowServerNameError(ServerName);
		return false;
	}

	/*
	*	Verify number of players 
	*
	*	range 2 - 99
	*/
	const FRegexPattern PlayerNumberPattern(TEXT("^[2-9][0-9]{0,1}$"));
	FRegexMatcher PlayerNumberMatcher(PlayerNumberPattern, PlayerNumber);
	if (!PlayerNumberMatcher.FindNext()) {
		ShowPlayerNumberError(PlayerNumber);
		return false;
	}

	return true;
}

void UHostWidget::ShowServerNameError(const FString& InServerName)
{
	FString ErrorMessage;
	if (InServerName.IsEmpty())
		ErrorMessage = "Server name cannot be empty.";
	else if (InServerName.Len() < 3 || InServerName.Len() > 20)
		ErrorMessage = "Server name must be between 3 and 20 characters long.";
	else if (!FChar::IsAlpha(InServerName[0]))
		ErrorMessage = "Server name must start with a letter (A-Z or a - z).";
	else
		ErrorMessage = "Server name contains invalid characters.\nOnly alphabets, numbers and underscores are allowed.";

	WidgetUtils::ShowError(this, ErrorMessage);
}

void UHostWidget::ShowPlayerNumberError(const FString& InPlayerNumber)
{
	FString ErrorMessage;
	if (InPlayerNumber.IsEmpty())
		ErrorMessage = "Player number cannot be empty.";
	else
		ErrorMessage = "Invalid player number.\nValid number is in between range of 2 - 99.";

	WidgetUtils::ShowError(this, ErrorMessage);
}

