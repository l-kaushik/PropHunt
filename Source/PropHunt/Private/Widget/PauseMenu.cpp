// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PauseMenu.h"
#include "Controller/PropHuntPlayerController.h"

// NOTE: Handle pause menu creation after game end

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetIsFocusable(true);
	PlayerController = Cast<APropHuntPlayerController>(GetOwningPlayer());
}

FReply UPauseMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();

	if (Key == EKeys::Escape || Key == EKeys::P)
	{
		if (!PlayerController) return FReply::Unhandled();

		PlayerController->TogglePauseMenu();

		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}
