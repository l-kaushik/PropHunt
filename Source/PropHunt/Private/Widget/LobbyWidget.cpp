// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LobbyWidget.h"
#include "Controller/MenuController.h"
#include "Utils/PropHuntLog.h"

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
	if (ReadyOrStartButton)
	{
		ReadyOrStartButton->OnClicked.AddDynamic(this, &ThisClass::OnReadyOrStartButtonClicked);
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &ThisClass::OnBackButtonClicked);
	}
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
}

void ULobbyWidget::OnBackButtonClicked()
{
	if (auto* MenuController = GetOwningPlayer<AMenuController>())
	{
		MenuController->ClientWantsToQuit();
	}
}

void ULobbyWidget::InitializeComponents()
{
	InitializeReadyOrStartButtonText();
}

void ULobbyWidget::InitializeReadyOrStartButtonText()
{
	if (IsHost)
	{
		ReadyOrStartButtonText->SetText(FText::FromString("Start Game"));
	}
	else
	{
		ReadyOrStartButtonText->SetText(FText::FromString("Ready"));
	}
}
