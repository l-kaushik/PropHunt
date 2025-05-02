// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MenuController.h"
#include "Widget/UIManager.h"
#include "Widget/MenuWidget.h"
#include "Widget/HostWidget.h"
#include "Widget/JoinGameWidget.h"
#include "Widget/LobbyWidget.h"
#include "Widget/PlayerEntryWidget.h"
#include "Widget/ServerEntryWidget.h"
#include "Widget/ErrorBox/UIErrorBox.h"
#include "Widget/LoadingScreenWidget.h"
#include "GameModes/MenuGameMode.h"
#include "GameInstance/PropHuntGameInstance.h"
#include "States/PropHuntPlayerState.h"
#include "States/PropHuntGameState.h"
#include "Utils/WidgetUtils.h"
#include "Utils/PropHuntLog.h"
#include "Utils/MapManager.h"
#include "Utils/Struct.h"

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
	LoadingScreenWidgetBPClassRef = UUIManager::Get()->LoadingScreenWidgetBPClassRef;

	// initialize variables
	IsPlayerListUpdateTimerOn = false;
}

void AMenuController::BeginPlay()
{
	Super::BeginPlay();

	// Initialize variables
	PropHuntGameInstance = Cast<UPropHuntGameInstance>(GetWorld()->GetGameInstance());
	PropHuntGameState = GetWorld()->GetGameState<APropHuntGameState>();
	PropHuntMenuGameMode = Cast<AMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	PropHuntGameState->OnPlayerListUpdated.AddUObject(this, &ThisClass::OnPlayerListUpdated);

	if (IsLocalPlayerController()){

		MenuWidgetRef = WidgetUtils::CreateAndAddWidget<UMenuWidget>(this, MenuWidgetBPClassRef);
		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);

		// create lobby widget on top of menu widget if its multiplayer game
		if (PropHuntGameInstance->GetIsMultiplayer())
		{
			if (PropHuntGameInstance->GetIsHost())
			{
				auto* CustomPlayerState = GetPlayerState<APropHuntPlayerState>();

				// if multiplayer game, set host variable to true for host
				CustomPlayerState->SetIsHost(PropHuntGameInstance->GetIsHost());

				// if this is host and make it always be ready
				CustomPlayerState->SetIsReady(true);

				// set mapinfo in game state so other client can have it
				PropHuntGameState->SetMapInfo(PropHuntGameInstance->GetMapInfo());
			}
			else
			{
			// request map info from server to store in game instance (for all clients)
				ServerRequestMapInfo();
			}

			SetupWidgetForMuliplayer();
			OnPlayerListUpdated(PropHuntGameState->GetPlayerStates());	// explicitly calling to update widget for host as well
		}

		const FString DisconnectReason = PropHuntGameInstance->GetLastDisconnectReason();

		if (!DisconnectReason.IsEmpty())
		{
			DisplaySessionError(DisconnectReason);
			PropHuntGameInstance->SetLastDisconnectReason(FText());
		}

	}
}

void AMenuController::ClientReturnToMainMenuWithTextReason_Implementation(const FText& ReturnReason)
{
	PropHuntGameInstance->SetLastDisconnectReason(ReturnReason);
	Super::ClientReturnToMainMenuWithTextReason_Implementation(ReturnReason);
}

void AMenuController::OnPlayerListUpdated(const TArray<APropHuntPlayerState*> &PlayerStates)
{
	if (!LobbyWidgetRef) return;
	
	LobbyWidgetRef->ClearPlayerList();

	bool bAllReady = true;

	for(auto* EachPlayerState : PlayerStates)
	{
		if (EachPlayerState)
		{
			// add player to widget list
			AddNewPlayerToList(EachPlayerState);
			
			// enable/disable start button if this player is host and the current player state shows not ready
			if (PropHuntGameInstance->GetIsHost() && !EachPlayerState->GetIsReady())
			{
				bAllReady = false;
			}
		}
	}

	if (PropHuntGameInstance->GetIsHost())
	{
		UpdateStartButtonState(bAllReady);
	}

	StartPlayerListUpdateTimer();
}

void AMenuController::DisplaySessionError(const FString& ErrorMessage)
{
	WidgetUtils::ShowError(this, ErrorMessage);
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
		0.5f,
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

void AMenuController::AddNewPlayerToList(const APropHuntPlayerState* InPlayerState)
{
	if (auto* PlayerEntryWidgetRef = WidgetUtils::CreateAndValidateWidget<UPlayerEntryWidget>(this, PlayerEntryWidgetBPClassRef))
	{
		PlayerEntryWidgetRef->SetPlayerNameText(InPlayerState->GetPlayerName());
		PlayerEntryWidgetRef->SetPingText(FString::FromInt(InPlayerState->GetPingInMilliseconds()));
		PlayerEntryWidgetRef->SetReadyStatus(InPlayerState->GetIsReady());

		LobbyWidgetRef->AddPlayerToList(PlayerEntryWidgetRef);;
	}
}

void AMenuController::UpdateStartButtonState(bool IsReady)
{
	if (!LobbyWidgetRef)
	{
		UE_LOG(LogPropHuntMenuController, Error, TEXT("Lobby widget reference is invalid in UpdateStartButtonState"));
		return;
	}

	if (IsReady)
	{
		LobbyWidgetRef->EnableStartButton();
	}
	else
	{
		LobbyWidgetRef->DisableStartButton();
	}
}

void AMenuController::ClientWantsToHost(const FName& SessionName, const FMapInfo& MapInfo, int32 NumPublicConnections, bool IsLANMatch)
{
	ClientWantsToHostOnServer(SessionName, MapInfo, NumPublicConnections, IsLANMatch);
}

void AMenuController::ClientWantsToHostOnServer_Implementation(const FName& SessionName, const FMapInfo& MapInfo, int32 NumPublicConnections, bool IsLANMatch)
{
	PropHuntGameInstance->HostSession(SessionName, MapInfo, NumPublicConnections, IsLANMatch);
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
	ShowLoadingScreen("Joining lobby");
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

void AMenuController::ShowLoadingScreen_Implementation(const FString& InMessage = "Loading Map")
{
	UE_LOG(LogTemp, Warning, TEXT("Show loading screen called"));

	LoadingWidget = WidgetUtils::CreateAndAddWidget<ULoadingScreenWidget>(this, LoadingScreenWidgetBPClassRef);
	LoadingWidget->SetLoadingMessage(InMessage);
}

void AMenuController::HostWantsToStartGameOnServer_Implementation()
{
	// show loading screen for all connected clients
	for (const auto& Controller : PropHuntGameState->GetMenuPlayerControllerList())
	{
		Controller->ShowLoadingScreen("Travelling to server...");
	}

	//perform server travel after .5 seconds
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle, 
		[this]() {
		PropHuntGameInstance->SetPlayerNum(PropHuntGameState->PlayerArray.Num());
		PropHuntGameInstance->StartSession();
		},
		0.5f,
		false
	);
}

void AMenuController::HostWantsToQuit()
{
	//PropHuntMenuGameMode->ReturnToMainMenuHost();

	ClientWantsToQuit();
}

void AMenuController::ClientWantsToQuit()
{
	UE_LOG(LogTemp, Warning, TEXT("One player quit"));

	ClientTravel(MapManager::Map_Menu, ETravelType::TRAVEL_Absolute);
	PropHuntGameInstance->QuitGameCleanup();
	PropHuntGameInstance->DestroySession();
}

void AMenuController::UpdateClientReadyStatus(bool IsReady)
{
	ServerUpdateReadyStatus(IsReady);
}

// for now depends on replication to disable host's ready button
void AMenuController::ServerUpdateReadyStatus_Implementation(const bool IsReady)
{
	auto* CustomPlayerState = GetPlayerState<APropHuntPlayerState>();
	CustomPlayerState->SetIsReady(IsReady);
}

void AMenuController::ServerRequestMapInfo_Implementation()
{
	ClientReceiveMapInfo(PropHuntGameState->GetMapInfo());
}

void AMenuController::ClientReceiveMapInfo_Implementation(const FMapInfo& MapInfo)
{
	PropHuntGameInstance->SetMapInfo(MapInfo);
}