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
		SetActiveOption(SelectableOptions[CurrentOption]);
}

void USelectionBox::SetOptions(const TArray<FString>& InOptions)
{
	SelectableOptions.Append(InOptions);
}

void USelectionBox::SetActiveOption(int32 OptionIndex)
{
	if ((OptionIndex >= 0) && (OptionIndex < SelectableOptions.Num()))
	{
		SetActiveOptionTextOnly(OptionIndex);
		NotifySelectionChanged(SelectableOptions[OptionIndex]);
	}
}

void USelectionBox::SetActiveOptionTextOnly(int32 OptionIndex)
{
	if ((OptionIndex >= 0) && (OptionIndex < SelectableOptions.Num()))
	{
		SelectionText->SetText(FText::FromString(SelectableOptions[OptionIndex]));
		CurrentOption = OptionIndex;
	}
}

void USelectionBox::NotifySelectionChanged(const FString& SelectedItem) const
{
	OnSelectionChanged.Broadcast(SelectedItem);
}

int32 USelectionBox::GetSelectedOptionIndex() const
{
	return CurrentOption;
}

const FString& USelectionBox::GetSelectedOptionString() const
{
	return SelectableOptions[CurrentOption];
}

void USelectionBox::SetActiveOption(const FString& InOption)
{
	if (SelectableOptions.Contains(InOption))
	{
		SetActiveOptionTextOnly(InOption);
		NotifySelectionChanged(InOption);
	}
}

void USelectionBox::SetActiveOptionTextOnly(const FString& InOption)
{
	if (SelectableOptions.Contains(InOption))
	{
		SelectionText->SetText(FText::FromString(InOption));
		CurrentOption = SelectableOptions.Find(InOption);
	}
}

void USelectionBox::OnLeftArrowButtonClicked()
{
	// Decrement and wrap around
	if (SelectableOptions.Num() > 0)
	{
		CurrentOption = (CurrentOption - 1 + SelectableOptions.Num()) % SelectableOptions.Num();
		SetActiveOption(SelectableOptions[CurrentOption]);
	}
}

void USelectionBox::OnRightArrowButtonClicked()
{
	// Increment and wrap around
	if (SelectableOptions.Num() > 0)
	{
		CurrentOption = (CurrentOption + 1) % SelectableOptions.Num();
		SetActiveOption(SelectableOptions[CurrentOption]);
	}
}
