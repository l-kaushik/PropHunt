// Fill out your copyright notice in the Description page of Project Settings.

#include "Controller/PropHuntPlayerController.h"
#include "Widget/MainHud.h"
#include "Widget/UIManager.h"
#include "Widget/LoadingScreenWidget.h"
#include "GameInstance/PropHuntGameInstance.h"
#include "GameModes/PropHuntGameMode.h"
#include "States/PropHuntGameState.h"
#include "Utils/MapManager.h"
#include "Utils/WidgetUtils.h"

#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

APropHuntPlayerController::APropHuntPlayerController() {
	static ConstructorHelpers::FClassFinder<UUserWidget> MainHudBPClass(TEXT("/Game/Widgets/WB_MainHud"));
	if (MainHudBPClass.Succeeded()) {
		MainHudBPClassRef = MainHudBPClass.Class;
	}

	LoadingScreenWidgetBPClassRef = UUIManager::Get()->LoadingScreenWidgetBPClassRef;

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
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);
	}

	PropHuntGameInstance = GetWorld()->GetGameInstance<UPropHuntGameInstance>();
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

void APropHuntPlayerController::ShowWinScreenWidget()
{
	ShowWinScreenOnClient();
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

void APropHuntPlayerController::SetIsProp(bool bIsProp)
{
	m_bIsProp = bIsProp;
}

void APropHuntPlayerController::UpdateWeaponUI(int32 InCurrentAmmoInMagazine, int32 InCurrentReserverAmmo)
{
	MainHudRef->UpdateWeaponUI(InCurrentAmmoInMagazine, InCurrentReserverAmmo);
}

bool APropHuntPlayerController::GetIsHost()
{
	return PropHuntGameInstance->GetIsHost();
}

void APropHuntPlayerController::StartNewGame()
{
	auto* GameMode = Cast<APropHuntGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode)
	{
		ServerInitiateLoadingScreen();
		GameMode->StartNextGame();
	}
}

void APropHuntPlayerController::ExitGame()
{
	// same calls are present in MenuController so find a way to make it resuable and modular

	// remove player from player list, reduce number of players in game instance and state as well

	ExitClientOnServer();
}

void APropHuntPlayerController::ExitClient()
{
	ClientTravel(MapManager::Map_Menu, ETravelType::TRAVEL_Absolute);
	PropHuntGameInstance->QuitGameCleanup();
	PropHuntGameInstance->DestroySession();
}

void APropHuntPlayerController::ExitClientOnServer_Implementation()
{
	auto* GameMode = Cast<APropHuntGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode)
	{
		GameMode->CleanupPlayerExitFromScoreboard();
	}

	ShowLoadingScreen("Returning to main menu");
	ExitClient();
	UE_LOG(LogTemp, Warning, TEXT("One player quit"));
}

void APropHuntPlayerController::ShowHitMarkerOnClient_Implementation()
{
	MainHudRef->PlayHitMarkerAnimation();
}

void APropHuntPlayerController::UpdateHealthOnClient_Implementation(float NewHealth)
{
	MainHudRef->UpdateHealthBar(NewHealth);
}

void APropHuntPlayerController::ShowWinScreenOnClient_Implementation()
{
	MainHudRef->ShowWinScreen(PropHuntGameInstance->GetIsHost());

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MainHudRef->GetCachedWidget());
	SetInputMode(InputMode);
	bShowMouseCursor = true;
}

void APropHuntPlayerController::HandleHudWidgetOnClient_Implementation(bool bIsProp)
{
	MainHudRef->SetupPropWidget(bIsProp);
}

void APropHuntPlayerController::ShowLoadingScreen_Implementation(const FString& InMessage = "Loading Map")
{
	UE_LOG(LogTemp, Warning, TEXT("Show loading screen called"));

	LoadingWidget = WidgetUtils::CreateAndAddWidget<ULoadingScreenWidget>(this, LoadingScreenWidgetBPClassRef);
	LoadingWidget->SetLoadingMessage(InMessage);
}

void APropHuntPlayerController::ServerInitiateLoadingScreen_Implementation()
{	// show loading screen for all connected clients

	auto* GameState = GetWorld()->GetGameState<APropHuntGameState>();

	for (const auto& Controller : GameState->GetPlayerControllerList())
	{
		Controller->ShowLoadingScreen("Travelling to server...");
	}
}
