// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HostWidget.h"
#include "Widget/MenuWidget.h"
#include "Controller/MenuController.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Widget/Components/Button/MasterButton.h"
#include "Internationalization/Regex.h"

void UHostWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BindEvents();

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
}

void UHostWidget::InitializeServerInfoVBox()
{
	if (ServerInfoVBox)
	{

	}
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
	FString LevelName = "ThirdPersonMap";
	// host server
	
	// use controller to host server
	MenuController->ClientWantsToHost(SessionName, LevelName, PlayerNumbers);

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
		UE_LOG(LogTemp, Warning, TEXT("Invalid server name: %s"), *ServerName);
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
		UE_LOG(LogTemp, Warning, TEXT("Invalid player number: %s"), *PlayerNumber);
		return false;
	}

	// display proper error message to user

	return true;
}

