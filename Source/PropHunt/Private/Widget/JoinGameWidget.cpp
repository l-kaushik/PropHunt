// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/JoinGameWidget.h"
#include "Widget/MenuWidget.h"
#include "Controller/MenuController.h"

#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/SizeBox.h"
#include "Components/CircularThrobber.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"

void UJoinGameWidget::AddServerToList(UUserWidget* ServerEntry)
{
	ServerListVBox->AddChild(ServerEntry);
}

void UJoinGameWidget::ClearServerList()
{
	ServerListVBox->ClearChildren();
}

void UJoinGameWidget::DisplaySessionLoadingThrobber()
{
	SearchSessionLoadingBox->SetVisibility(ESlateVisibility::Visible);
}

void UJoinGameWidget::HideSessionLoadingThrobber()
{
	SearchSessionLoadingBox->SetVisibility(ESlateVisibility::Hidden);
}

void UJoinGameWidget::DisplayNoSessionFoundMessage()
{
	SessionNotFoundBox->SetVisibility(ESlateVisibility::Visible);
}

void UJoinGameWidget::HideNoSessionFoundMessage()
{
	SessionNotFoundBox->SetVisibility(ESlateVisibility::Hidden);
}

void UJoinGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BindEvents();

	// hide by default
	HideNoSessionFoundMessage();
}

void UJoinGameWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UJoinGameWidget::BindEvents()
{
	if (RefreshButton)
	{
		RefreshButton->OnClicked.AddDynamic(this, &ThisClass::OnRefreshButtonClicked);
	}
}

void UJoinGameWidget::OnRefreshButtonClicked()
{
	static auto* MenuController = Cast<AMenuController>(GetOwningPlayer());

	if (MenuController)
	{
		HideNoSessionFoundMessage();
		DisplaySessionLoadingThrobber();
		MenuController->SearchSessions();
	}
}
