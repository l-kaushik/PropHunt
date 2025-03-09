// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ErrorBox/UIErrorBox.h"
#include "Widget/Components/Button/MasterButton.h"
#include "Utils/PropHuntLog.h"

#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/ScaleBox.h"
#include "Components/TextBlock.h"

void UUIErrorBox::SetMessage(const FString& InMessage)
{
	if (ErrorMessageTextBox)
	{
		ErrorMessageTextBox->SetText(FText::FromString(InMessage));
	}
	else
	{
		UE_LOG(LogPropHuntWidget, Warning, TEXT("ErrorMessageTextBox is nullptr"));
	}
}

void UUIErrorBox::SetOnOkClicked(TFunction<void()> InCallback)
{
	OnOkCallback = InCallback;
}

void UUIErrorBox::NativeConstruct()
{
	Super::NativeConstruct();

	BindClickEvents();
}

void UUIErrorBox::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (OkButton)
	{
		OkButton->SetLabel("Ok");
	}
}

void UUIErrorBox::BindClickEvents()
{
	BIND_BUTTON_CLICK(OkButton, &ThisClass::OnOkButtonClicked);
}

void UUIErrorBox::OnOkButtonClicked()
{
	if (OnOkCallback) // Only call if it's set
	{
		OnOkCallback();
	}
	UWidget::RemoveFromParent();
}
