// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Components/SelectionBox.h"
#include "Widget/Components/Button/MasterButton.h"
#include "Macros/WidgetMacros.h"
#include "Utils/WidgetUtils.h"
#include "Utils/PropHuntLog.h"

#include "Components/TextBlock.h"

void USelectionBox::NativeConstruct()
{
	Super::NativeConstruct();

	BIND_BUTTON_CLICK(LeftArrowButton, &USelectionBox::OnLeftArrowButtonClicked);
	BIND_BUTTON_CLICK(RightArrowButton, &USelectionBox::OnRightArrowButtonClicked);
}

void USelectionBox::NativePreConstruct()
{
	Super::NativePreConstruct();

	LeftArrowButton->SetLabel("<");
	RightArrowButton->SetLabel(">");

	if(SelectionText && SelectableOptions.Num() > 0)
		SetSelectOptions(SelectableOptions[CurrentOption]);
}

void USelectionBox::SetOptions(const TArray<FString>& InOptions)
{
	SelectableOptions.Append(InOptions);
}

void USelectionBox::NotifySelectionChanged(const FString& SelectedItem)
{
	OnSelectionChanged.Broadcast(SelectedItem);
}

void USelectionBox::SetSelectOptions(const FString& InOption)
{
	if (SelectableOptions.Contains(InOption))
	{
		SelectionText->SetText(FText::FromString(InOption));
		NotifySelectionChanged(InOption);
	}
}

void USelectionBox::OnLeftArrowButtonClicked()
{
	// Decrement and wrap around
	if (SelectableOptions.Num() > 0)
	{
		CurrentOption = (CurrentOption - 1 + SelectableOptions.Num()) % SelectableOptions.Num();
		SetSelectOptions(SelectableOptions[CurrentOption]);
	}
}

void USelectionBox::OnRightArrowButtonClicked()
{
	// Increment and wrap around
	if (SelectableOptions.Num() > 0)
	{
		CurrentOption = (CurrentOption + 1) % SelectableOptions.Num();
		SetSelectOptions(SelectableOptions[CurrentOption]);
	}
}
