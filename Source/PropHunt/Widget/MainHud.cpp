// Fill out your copyright notice in the Description page of Project Settings.

#include "MainHud.h"
#include "../States/PropHuntGameState.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/BackgroundBlur.h"
#include "Components/Border.h"
#include "Animation/WidgetAnimation.h"
#include "Engine/World.h"

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

void UMainHud::PlayHitMarkerAnimation()
{
	if (HitMarker && HitMarkerAnim)
	{
		HitMarker->SetVisibility(ESlateVisibility::Visible);
		UUserWidget::PlayAnimation(HitMarkerAnim);

		// Bind the animation finished event
		FWidgetAnimationDynamicEvent AnimationFinishedDelegate;
		AnimationFinishedDelegate.BindUFunction(this, FName("HitMarkerAnimFinished"));
	
		UUserWidget::BindToAnimationFinished(HitMarkerAnim, AnimationFinishedDelegate);
	}
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

void UMainHud::StartTimer()
{
	// timer to keep track of remaining game time
	GetWorld()->GetTimerManager().SetTimer(
		CountdownTimer,
		this,
		&UMainHud::EndTimer,
		APropHuntGameState::GAME_TIME_IN_SECONDS,
		false
	);

	// timer to update timer text every second
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&UMainHud::UpdateTimerText,
		1.0f,
		true
	);
}

void UMainHud::EndTimer()
{
}

void UMainHud::UpdateTimerText()
{
	float RemainingSeconds = GetWorld()->GetTimerManager().GetTimerRemaining(CountdownTimer);
	FTimespan RemainingTime = FTimespan::FromSeconds(RemainingSeconds);
	int32 Minutes = RemainingTime.GetMinutes();
	int32 Seconds = RemainingTime.GetSeconds();
	FString TimerString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	TimerText->SetText(FText::FromString(TimerString));;
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
	SetTimerBorder();
	SetTimerIcon();
	SetTimerText();
	SetHitMarker();
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

void UMainHud::SetHitMarker()
{
	if (HitMarker)
	{
		if (UTexture2D* HitMarkerTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/ThirdPerson/Widgets/Textures/crosshair.crosshair"))) {
			HitMarker->SetBrushFromTexture(HitMarkerTexture);
		}
		HitMarker->SetVisibility(ESlateVisibility::Hidden);
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

void UMainHud::SetTimerBorder()
{
	if (TimerBorder)
	{
		FLinearColor BlackWithAlpha(0.0f, 0.0f, 0.0f, 0.5f);
		TimerBorder->SetBrushColor(BlackWithAlpha);	// tint
	}
}

void UMainHud::SetTimerIcon()
{
	if (TimerIcon)
	{
		if (UTexture2D* TimerIconTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/ThirdPerson/Widgets/Textures/timericon.timericon"))) {
			TimerIcon->SetBrushFromTexture(TimerIconTexture);
		}
	}
}

void UMainHud::SetTimerText()
{
	if (TimerText)
	{
		FSlateFontInfo FontInfo = TimerText->GetFont();
		FontInfo.Size = 45;

		TimerText->SetFont(FontInfo);
		TimerText->SetText(FText::FromString("5:00"));
	}
}

void UMainHud::HitMarkerAnimFinished(UWidgetAnimation* Animation)
{
	if(Animation == HitMarkerAnim)
		HitMarker->SetVisibility(ESlateVisibility::Hidden);
}
