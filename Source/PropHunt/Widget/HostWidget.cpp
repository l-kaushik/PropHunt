// Fill out your copyright notice in the Description page of Project Settings.


#include "HostWidget.h"
#include "../Widget/MenuWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

void UHostWidget::SetParentWidget(UMenuWidget* InParentWidget)
{
	ParentWidget = InParentWidget;
}

void UHostWidget::NativeConstruct()
{
	Super::NativeConstruct();


	if (ServerNameLabel)
	{
		ServerNameLabel->SetText(FText::FromString("Server Name: "));
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UHostWidget::OnBackButtonClicked);
	}
}

void UHostWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (ServerNameLabel)
	{
		ServerNameLabel->SetText(FText::FromString("Server Name: "));
	}
}

void UHostWidget::OnBackButtonClicked()
{
	this->RemoveFromParent();

	if (ParentWidget)
	{
		ParentWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
