// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "MainHud.h"

void UMainHud::SetupPropWidget(bool bIsProp)
{
	FText GameStatusText;

	if (bIsProp) {
		GameStatusText = FText::FromString("You are the Prop! Hide!");
		Crosshair->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		GameStatusText = FText::FromString("Find the Prop!");
		Crosshair->SetVisibility(ESlateVisibility::Visible);
	}

	GameStatus->SetText(GameStatusText);
		
}

// Set things for the gameplay time
void UMainHud::NativeConstruct()
{
	Super::NativeConstruct();

	SetGameStatusText();
	SetCrosshairImage();
}

// Set things for the UMG blueprint design preview
void UMainHud::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetGameStatusText();
	SetCrosshairImage();
}

void UMainHud::SetGameStatusText()
{
	if (GameStatus) {
		GameStatus->SetText(FText::FromString("Waiting for players to join..."));
	}
}

void UMainHud::SetCrosshairImage()
{
	if (Crosshair)
	{
		if (UTexture2D* CrosshairTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/ThirdPerson/Widgets/Textures/crosshair.crosshair"))) {
			Crosshair->SetBrushFromTexture(CrosshairTexture);
		}
	}
}
