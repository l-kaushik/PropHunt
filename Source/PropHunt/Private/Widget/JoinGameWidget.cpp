// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/JoinGameWidget.h"
#include "Widget/MenuWidget.h"

#include "Components/Button.h"
#include "Components/VerticalBox.h"

void UJoinGameWidget::SetParentWidget(UMenuWidget* InParentWidget)
{
	ParentWidget = InParentWidget;
}

void UJoinGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BindEvents();
}

void UJoinGameWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UJoinGameWidget::BindEvents()
{
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &ThisClass::OnBackButtonClicked);
	}
}

void UJoinGameWidget::OnBackButtonClicked()
{
	this->RemoveFromParent();
	if (ParentWidget)
	{
		ParentWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
