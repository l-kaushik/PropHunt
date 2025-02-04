// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Components/Button/MasterButton.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"



void UMasterButton::SetLabel(FString NewLabel)
{
	ButtonLabel->SetText(FText::FromString(NewLabel));
}

void UMasterButton::SetBackgroundColor(FLinearColor NewColor)
{
	Button->SetBackgroundColor(NewColor);
}

void UMasterButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button && !bEventBound)
	{
		Button->OnClicked.AddDynamic(this, &UMasterButton::HandleButtonClicked);
		bEventBound = true;
	}
}

void UMasterButton::HandleButtonClicked()
{
	OnClicked.Broadcast();
}
