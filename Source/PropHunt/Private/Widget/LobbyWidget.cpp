// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LobbyWidget.h"
#include "Widget/Components/Button/MasterButton.h"
#include "Widget/PlayerEntryWidget.h"
#include "Controller/MenuController.h"
#include "States/PropHuntPlayerState.h"
#include "Utils/PropHuntLog.h"
#include "Macros/WidgetMacros.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void ULobbyWidget::SetIsHost(bool bIsHost)
{
	IsHost = bIsHost;
	InitializeReadyOrStartButtonText();
}

bool ULobbyWidget::GetIsHost()
{
	return IsHost;
}

void ULobbyWidget::DisableStartButton()
{
	ReadyOrStartButton->SetIsEnabled(false);
}

void ULobbyWidget::EnableStartButton()
{
	ReadyOrStartButton->SetIsEnabled(true);
}

void ULobbyWidget::SetParentWidget(UUserWidget* InParentWidget)
{
	ParentWidget = InParentWidget;
}

void ULobbyWidget::AddPlayerToList(UUserWidget* PlayerWidget)
{
	PlayersListVBox->AddChild(PlayerWidget);
}

void ULobbyWidget::ClearPlayerList()
{
	PlayersListVBox->ClearChildren();
}

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeComponents();
	BindEvents();
}

void ULobbyWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	InitializeComponents();
}

void ULobbyWidget::BindEvents()
{
	BIND_BUTTON_CLICK(ReadyOrStartButton, &ThisClass::OnReadyOrStartButtonClicked);
	BIND_BUTTON_CLICK(BackButton, &ThisClass::OnBackButtonClicked);
}

void ULobbyWidget::OnReadyOrStartButtonClicked()
{
	auto* MenuController = GetOwningPlayer<AMenuController>();
	
	if (!MenuController)
	{
		UE_LOG(LogPropHuntWidget, Error, TEXT("Failed to get menu controller"));
		return;
	}

	if (IsHost)
	{
		MenuController->HostWantsToStartGame();
	}
	else
	{
		// toggle ready button stats
		bool IsReady = !MenuController->GetPlayerState<APropHuntPlayerState>()->GetIsReady();
		FString readyText = IsReady ? FString("Unready") : FString("Ready");
		ReadyOrStartButton->SetLabel(readyText);
		MenuController->UpdateClientReadyStatus(IsReady);
	}
}

void ULobbyWidget::OnBackButtonClicked()
{
	if (auto* MenuController = GetOwningPlayer<AMenuController>())
	{
		if(IsHost)
			MenuController->HostWantsToQuit();
		else
			MenuController->ClientWantsToQuit();
	}
}

void ULobbyWidget::InitializeComponents()
{
	if (BackButton)
	{
		BackButton->SetLabel("X");
	}

	InitializeReadyOrStartButtonText();
	InitializePlayersListHeader();
}

void ULobbyWidget::InitializeReadyOrStartButtonText()
{
	if (IsHost)
	{
		ReadyOrStartButton->SetLabel(FString("Start Game"));
	}
	else
	{
		ReadyOrStartButton->SetLabel(FString("Ready"));
	}

}

void ULobbyWidget::InitializePlayersListHeader()
{
	PlayersListHeader->SetPlayerNameText(FString("Name"));
	PlayersListHeader->SetPingText(FString("Pings"));
	PlayersListHeader->SetReadyStatusText(FString("Status"));
}
