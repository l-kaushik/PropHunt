// Fill out your copyright notice in the Description page of Project Settings.

#include "PropHuntPlayerController.h"
#include "../Widget/MainHud.h"

#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

APropHuntPlayerController::APropHuntPlayerController() {
	static ConstructorHelpers::FClassFinder<UUserWidget> MainHudBPClass(TEXT("/Game/ThirdPerson/Widgets/WB_MainHud"));
	if (MainHudBPClass.Succeeded()) {
		MainHudBPClassRef = MainHudBPClass.Class;
	}

	// intitial values
	m_bIsProp = false;
}

void APropHuntPlayerController::BeginPlay() {
	Super::BeginPlay();

	/*
	* In "Listen server architecture"
	* HasAuthority() && IsLocalPlayerController() --> this line checks, if listen server is a client also, if it is then create UI for it
	* the || !HasAuthority() -->this line create UI for remote player
	* 
	* Hence, it ensure all client gets UI
	*/

	if ((HasAuthority() && IsLocalPlayerController()) || !HasAuthority()) {
		if (!MainHudBPClassRef) return;

		MainHudRef = CreateWidget<UMainHud>(this, MainHudBPClassRef);
		check(MainHudRef);
		MainHudRef->AddToPlayerScreen();
	}
}

void APropHuntPlayerController::SetupPropWidget(bool bIsProp)
{
	HandleHudWidgetOnClient(bIsProp);
	this->m_bIsProp = bIsProp;
}

void APropHuntPlayerController::UpdateHealthWidget(float NewHealth)
{
	UpdateHealthOnClient(NewHealth);
}

void APropHuntPlayerController::ShowWinScreenWidget(bool bIsPropWon)
{
	ShowWinScreenOnClient(bIsPropWon, m_bIsProp);
}

void APropHuntPlayerController::StartCountdownWidget()
{
	StartCountdownOnClient();
}

void APropHuntPlayerController::PlayHitMarkerAnim()
{
	ShowHitMarkerOnClient();
}

void APropHuntPlayerController::StartCountdownOnClient_Implementation()
{
	MainHudRef->StartTimer();
}


void APropHuntPlayerController::TrySetupPropWidget(bool bIsProp)
{
	if (IPropHuntControllerInterface* PropHuntInterface = Cast<IPropHuntControllerInterface>(this))
	{
		PropHuntInterface->SetupPropWidget(bIsProp);
	}
}

void APropHuntPlayerController::ShowHitMarkerOnClient_Implementation()
{
	MainHudRef->PlayHitMarkerAnimation();
}

void APropHuntPlayerController::UpdateHealthOnClient_Implementation(float NewHealth)
{
	MainHudRef->UpdateHealthBar(NewHealth);
}

void APropHuntPlayerController::ShowWinScreenOnClient_Implementation(bool bIsPropWon, bool bIsProp)
{
	MainHudRef->ShowWinScreen(bIsPropWon, bIsProp);
}

void APropHuntPlayerController::HandleHudWidgetOnClient_Implementation(bool bIsProp)
{
	MainHudRef->SetupPropWidget(bIsProp);
}
