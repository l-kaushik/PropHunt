// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LobbyWidget.h"

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
}

void ULobbyWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	InitializeComponents();
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
