// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/BackgroundBlur.h"
#include "MainHud.h"

void UMainHud::SetupPropWidget(bool bIsProp)
{
	FText GameStatusText;

	if (bIsProp) {
		GameStatusText = FText::FromString("You are the Prop! Hide!");
		HealthBar->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		GameStatusText = FText::FromString("Find the Prop!");
		Crosshair->SetVisibility(ESlateVisibility::Visible);
	}

	GameStatus->SetText(GameStatusText);
		
}

void UMainHud::UpdateHealthBar(float NewHealth)
{
	HealthBar->SetPercent(NewHealth * 0.01);
}

void UMainHud::ShowWinScreen(bool bIsPropWon, bool bIsProp)
{
	FString text;
	FSlateColor textColor;

	WinScreen->SetVisibility(ESlateVisibility::Visible);

	if (bIsPropWon)
	{
		if (bIsProp)
		{
			text = "You Won! You Survived The Hunters!;";
			textColor = FLinearColor::Green;
		}
		else
		{
			text = "You Lost! The Prop Won!";
			textColor = FLinearColor::Red;
		}

	}
	else 
	{
		if (bIsProp)
		{
			text = "You Lost! Prop Hunters Win!";
			textColor = FLinearColor::Red;
		}
		else
		{
			text = "You Won! You Found The Prop!";
			textColor = FLinearColor::Green;
		}
	}

	WinScreenText->SetText(FText::FromString(text));
	WinScreenText->SetColorAndOpacity(textColor);
}

// Set things for the gameplay time
void UMainHud::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeWidgetComponents();
}

// Set things for the UMG blueprint design preview
void UMainHud::NativePreConstruct()
{
	Super::NativePreConstruct();

	InitializeWidgetComponents();

	// gameplay only settings
	WinScreen->SetVisibility(ESlateVisibility::Hidden);
}

void UMainHud::InitializeWidgetComponents()
{
	SetGameStatusText();
	SetCrosshairImage();
	SetHealthBar();
	SetWinScreen();
	SetWinScreenText();
	SetNewGameStartingText();
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
		Crosshair->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainHud::SetHealthBar()
{
	if (HealthBar)
	{
		HealthBar->SetPercent(1.0f);
		HealthBar->SetFillColorAndOpacity(FLinearColor::Red);
		HealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainHud::SetWinScreen()
{
	if (WinScreen)
	{
		WinScreen->SetBlurStrength(30.f);
	}
}

void UMainHud::SetWinScreenText()
{
	if (WinScreenText)
	{
		FSlateFontInfo FontInfo = WinScreenText->GetFont();
		FontInfo.Size = 60;
		WinScreenText->SetFont(FontInfo);
		WinScreenText->SetText(FText::FromString("You Win!"));
		WinScreenText->SetJustification(ETextJustify::Center);
	}
}

void UMainHud::SetNewGameStartingText()
{
	if (NewGameStartingText)
	{
		FSlateFontInfo FontInfo = NewGameStartingText->GetFont();
		FontInfo.Size = 33;
		NewGameStartingText->SetFont(FontInfo);
		NewGameStartingText->SetText(FText::FromString("New Game Starting Soon..."));
		NewGameStartingText->SetJustification(ETextJustify::Center);
	}
}
