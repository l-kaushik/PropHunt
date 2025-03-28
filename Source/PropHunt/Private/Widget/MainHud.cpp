// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/MainHud.h"
#include "States/PropHuntGameState.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/BackgroundBlur.h"
#include "Components/Border.h"
#include "Animation/WidgetAnimation.h"
#include "Engine/World.h"

void UMainHud::SetupPropWidget(bool bIsProp)
{
	FText PlayerObjectiveText;

	if (bIsProp) {
		PlayerObjectiveText = FText::FromString("You are the Prop! Hide!");
		HealthBar->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		PlayerObjectiveText = FText::FromString("Find the Prop!");
		Crosshair->SetVisibility(ESlateVisibility::Visible);

		if(HealthBar->GetVisibility() == ESlateVisibility::Visible)
			HealthBar->SetVisibility(ESlateVisibility::Hidden);
	}

	PlayerObjective->SetText(PlayerObjectiveText);
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

void UMainHud::ShowWinScreen(bool bIsHost)
{
	FString text;
	FSlateColor textColor;

	WinScreen->SetVisibility(ESlateVisibility::Visible);
	
	// show game over and after 5-7 seconds, show leaderboard
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
	SetPlayerObjectiveText();
	SetCrosshairImage();
	SetHealthBar();
	SetWinScreen();
	SetWinScreenText();
	SetTimerBorder();
	SetTimerIcon();
	SetTimerText();
	SetHitMarker();
}

void UMainHud::SetPlayerObjectiveText()
{
	if (PlayerObjective) {
		PlayerObjective->SetText(FText::FromString("Waiting for players to join..."));
	}
}

void UMainHud::SetCrosshairImage()
{
	if (Crosshair)
	{
		if (UTexture2D* CrosshairTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Widgets/Textures/crosshair.crosshair"))) {
			Crosshair->SetBrushFromTexture(CrosshairTexture);
		}
		Crosshair->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainHud::SetHitMarker()
{
	if (HitMarker)
	{
		if (UTexture2D* HitMarkerTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Widgets/Textures/crosshair.crosshair"))) {
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
		FontInfo.OutlineSettings.OutlineSize = 5;
		WinScreenText->SetFont(FontInfo);
		WinScreenText->SetText(FText::FromString("Game Over!"));
		WinScreenText->SetColorAndOpacity(FLinearColor::Red);
		WinScreenText->SetJustification(ETextJustify::Center);
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
		if (UTexture2D* TimerIconTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Widgets/Textures/timericon.timericon"))) {
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
