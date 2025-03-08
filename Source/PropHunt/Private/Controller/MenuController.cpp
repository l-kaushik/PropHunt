// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MenuController.h"
#include "Widget/MenuWidget.h"
#include "Widget/HostWidget.h"
#include "Widget/JoinGameWidget.h"
#include "Widget/LobbyWidget.h"
#include "Widget/PlayerEntryWidget.h"
#include "Widget/ServerEntryWidget.h"
#include "GameModes/MenuGameMode.h"
#include "GameInstance/PropHuntGameInstance.h"
#include "States/PropHuntPlayerState.h"
#include "States/PropHuntGameState.h"
#include "Utils/WidgetUtils.h"
#include "Widget/UIManager.h"
#include "Utils/PropHuntLog.h"

#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "OnlineSessionSettings.h"

AMenuController::AMenuController()
{
	MenuWidgetBPClassRef = UUIManager::Get()->MenuWidgetBPClassRef;
	HostWidgetBPClassRef = UUIManager::Get()->HostWidgetBPClassRef;
	JoinGameWidgetBPClassRef = UUIManager::Get()->JoinGameWidgetBPClassRef;
	LobbyWidgetBPClassRef = UUIManager::Get()->LobbyWidgetBPClassRef;
	PlayerEntryWidgetBPClassRef = UUIManager::Get()->PlayerEntryWidgetBPClassRef;
	ServerEntryWidgetBPClassRef = UUIManager::Get()->ServerEntryWidgetBPClassRef;

	// initialize variables
	IsPlayerListUpdateTimerOn = false;
}

void AMenuController::BeginPlay()
{
	Super::BeginPlay();

	// Initialize variables
	PropHuntGameInstance = Cast<UPropHuntGameInstance>(GetWorld()->GetGameInstance());
	PropHuntGameState = GetWorld()->GetGameState<APropHuntGameState>();
	PropHuntGameState->OnPlayerListUpdated.AddUObject(this, &ThisClass::OnPlayerListUpdated);

	if (IsLocalPlayerController()){

		MenuWidgetRef = WidgetUtils::CreateAndAddWidget<UMenuWidget>(this, MenuWidgetBPClassRef);
		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);

		// create lobby widget on top of menu widget if its multiplayer game
		if (PropHuntGameInstance->GetIsMultiplayer())
		{
			SetupWidgetForMuliplayer();
			OnPlayerListUpdated(PropHuntGameState->GetPlayerStates());	// explicitly calling to update widget for host as well
		}
	}
}

void AMenuController::OnPlayerListUpdated(const TArray<APropHuntPlayerState*> &PlayerStates)
{
	UE_LOG(LogPropHuntMenuController, Display, TEXT("OnPlayerListUpdated called"));
	if (!LobbyWidgetRef) return;
	
	LobbyWidgetRef->ClearPlayerList();

	for(auto* EachPlayerState : PlayerStates)
	{
		UE_LOG(LogPropHuntMenuController, Display, TEXT("Added player to list"));
		if(EachPlayerState)
			AddNewPlayerToList(EachPlayerState->GetPlayerName(), FString::FromInt(EachPlayerState->GetPingInMilliseconds()));
	}

	StartPlayerListUpdateTimer();
}

void AMenuController::RefreshPlayerList()
{
	OnPlayerListUpdated(PropHuntGameState->GetPlayerStates());
}

void AMenuController::StartPlayerListUpdateTimer()
{
	// TODO: make sure to destroy when game starts
	if (IsPlayerListUpdateTimerOn) return;

	StopPlayerListUpdateTimer();
	IsPlayerListUpdateTimerOn = true;

	GetWorld()->GetTimerManager().SetTimer(
		PlayerListUpdateTImer,
		this,
		&AMenuController::RefreshPlayerList,
		2.0f,
		true
	);
}

void AMenuController::StopPlayerListUpdateTimer()
{
	IsPlayerListUpdateTimerOn = false;
	GetWorld()->GetTimerManager().ClearTimer(PlayerListUpdateTImer);
}

void AMenuController::HostWantsToStopPlayerListUpdateTimer_Implementation()
{
	StopPlayerListUpdateTimer();
}

void AMenuController::SetupWidgetForMuliplayer()
{
	LobbyWidgetRef = WidgetUtils::CreateSubWidgetAndHideParent<ULobbyWidget, UMenuWidget>(this, LobbyWidgetBPClassRef, MenuWidgetRef);
	LobbyWidgetRef->SetIsHost(PropHuntGameInstance->GetIsHost());
}

void AMenuController::AddNewPlayerToList(const FString& PlayerName, const FString& PingInms)
{
	if (auto* PlayerEntryWidgetRef = WidgetUtils::CreateAndValidateWidget<UPlayerEntryWidget>(this, PlayerEntryWidgetBPClassRef))
	{
		PlayerEntryWidgetRef->SetPlayerNameText(PlayerName);
		PlayerEntryWidgetRef->SetPingText(PingInms);

		LobbyWidgetRef->AddPlayerToList(PlayerEntryWidgetRef);;
	}
}

void AMenuController::ClientWantsToHost(const FName& SessionName, const FString& LevelName, int32 NumPublicConnections, bool IsLANMatch)
{
	ClientWantsToHostOnServer(SessionName, LevelName, NumPublicConnections, IsLANMatch);
}

void AMenuController::ClientWantsToHostOnServer_Implementation(const FName& SessionName, const FString& LevelName, int32 NumPublicConnections, bool IsLANMatch)
{
	PropHuntGameInstance->HostSession(SessionName, LevelName, NumPublicConnections, IsLANMatch);
}

void AMenuController::SearchSessions()
{
	SearchSessionsOnServer();
}

void AMenuController::SearchSessionsOnServer_Implementation()
{
	UE_LOG(LogPropHuntMenuController, Display, TEXT("Client sent a search session request"));
	PropHuntGameInstance->FindSessions(10);
}

void AMenuController::LoadSessionsInList(const TArray<FOnlineSessionSearchResult>& InSearchResults)
{
	if (InSearchResults.Num() < 0)
	{
		UE_LOG(LogPropHuntMenuController, Display, TEXT("No sessions found"));
		return;
	}

	// store session results array
	SearchResults = InSearchResults;
	AddServersToList();
}

void AMenuController::AddServersToList()
{
	// clear server list
	MenuWidgetRef->ClearServerList();
	MenuWidgetRef->HideSessionLoadingThrobber();

	// when search results are 0
	if (SearchResults.IsEmpty())
	{
		MenuWidgetRef->DisplayNoSessionFoundMessage();
	}

	for (int32 i = 0; i < SearchResults.Num(); i++)
	{
		// Check if the search result is valid
		if (!SearchResults[i].IsValid())
		{
			UE_LOG(LogPropHuntMenuController, Error, TEXT("Invalid session result."));
			continue;
		}

		FString ServerName("Test");
		SearchResults[i].Session.SessionSettings.Settings["SESSION_DISPLAY_NAME"].Data.GetValue(ServerName);
		int32 NumOpenPublicConnections = SearchResults[i].Session.NumOpenPublicConnections;
		int32 NumPublicConnections = SearchResults[i].Session.SessionSettings.NumPublicConnections;
		int32 Ping = SearchResults[i].PingInMs;
		FString Status = FString::Printf(TEXT("%d/%d"), NumPublicConnections - NumOpenPublicConnections, NumPublicConnections);

		if (auto* ServerEntryWidgetRef = WidgetUtils::CreateAndValidateWidget<UServerEntryWidget>(this, ServerEntryWidgetBPClassRef))
		{
			ServerEntryWidgetRef->SetServerNameText(ServerName);
			ServerEntryWidgetRef->SetPingText(FString::Printf(TEXT("%dms"), Ping));
			ServerEntryWidgetRef->SetStatusText(Status);
			ServerEntryWidgetRef->SetSessionResultIndex(i);

			MenuWidgetRef->AddServerToList(ServerEntryWidgetRef);
		}
	}

	UE_LOG(LogPropHuntMenuController, Display, TEXT("Sessions written to the list"));
}

void AMenuController::ClientWantsToJoin(int32 SessionResultIndex)
{
	ClientWantsToJoinOnServer(SessionResultIndex);
}

void AMenuController::ClientWantsToJoinOnServer_Implementation(int32 SessionResultIndex)
{
	UE_LOG(LogPropHuntMenuController, Display, TEXT("Join server button clicked"));
	FString ServerNameStr;
	FName SessionName(*ServerNameStr);
	FOnlineSessionSearchResult SessionResult = SearchResults[SessionResultIndex];
	SearchResults[SessionResultIndex].Session.SessionSettings.Settings["SESSION_DISPLAY_NAME"].Data.GetValue(ServerNameStr);

	PropHuntGameInstance->JoinGameSession(SessionName, SessionResult);
}

void AMenuController::HostWantsToStartGame()
{
	HostWantsToStartGameOnServer();
}

void AMenuController::HostWantsToStartGameOnServer_Implementation()
{
	PropHuntGameInstance->SetPlayerNum(PropHuntGameState->PlayerArray.Num());
	PropHuntGameInstance->StartSession();
}

void AMenuController::ClientWantsToQuit()
{
	UE_LOG(LogTemp, Warning, TEXT("One player quit"));

	StopPlayerListUpdateTimer();
	LobbyWidgetRef->RemoveFromParent();
	LobbyWidgetRef = nullptr;

	PropHuntGameInstance->QuitGameCleanup();
	ClientTravel("/Game/ThirdPerson/Maps/MenuMap", ETravelType::TRAVEL_Absolute);
}
