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

#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "OnlineSessionSettings.h"

AMenuController::AMenuController()
{
	FString BasePath = "/Game/Widgets/";

	MenuWidgetBPClassRef = LoadWidgetBlueprint<UMenuWidget>(BasePath + FString("WB_Menu"));
	HostWidgetBPClassRef = LoadWidgetBlueprint<UHostWidget>(BasePath + FString("WB_Host"));
	JoinGameWidgetBPClassRef = LoadWidgetBlueprint<UJoinGameWidget>(BasePath + FString("WB_JoinGame"));
	LobbyWidgetBPClassRef = LoadWidgetBlueprint<ULobbyWidget>(BasePath + FString("WB_Lobby"));
	PlayerEntryWidgetBPClassRef = LoadWidgetBlueprint<UPlayerEntryWidget>(BasePath + FString("WB_PlayerEntry"));
	ServerEntryWidgetBPClassRef = LoadWidgetBlueprint<UServerEntryWidget>(BasePath + FString("WB_ServerEntry"));
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
		}
	}
}

void AMenuController::OnPlayerListUpdated(const TArray<APropHuntPlayerState*> &PlayerStates)
{
	UE_LOG(LogTemp, Warning, TEXT("OnPlayerListUpdated called"));
	if (!LobbyWidgetRef) return;
	
	LobbyWidgetRef->ClearPlayerList();

	for(auto* EachPlayerState : PlayerStates)
	{
		UE_LOG(LogTemp, Warning, TEXT("Added player to list"));
		if(EachPlayerState)
			AddNewPlayerToList(EachPlayerState->GetPlayerName(), FString::FromInt(EachPlayerState->GetPingInMilliseconds()));
	}
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
	PropHuntGameInstance->FindSessions(10);
}

void AMenuController::LoadSessionsInList(const TArray<FOnlineSessionSearchResult>& InSearchResults)
{
	if (InSearchResults.Num() < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No sessions found"));
		return;
	}

	// store session results array
	SearchResults = InSearchResults;

	for (int32 i = 0; i < InSearchResults.Num(); i++)
	{
		// Check if the search result is valid
		if (!InSearchResults[i].IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid session result."));
			continue;
		}

		FString ServerName;
		InSearchResults[i].Session.SessionSettings.Settings["SESSION_DISPLAY_NAME"].Data.GetValue(ServerName);
		int32 NumOpenPublicConnections = InSearchResults[i].Session.NumOpenPublicConnections;
		int32 NumPublicConnections = InSearchResults[i].Session.SessionSettings.NumPublicConnections;
		int32 Ping = InSearchResults[i].PingInMs;
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
}

void AMenuController::ClientWantsToJoin(int32 SessionResultIndex)
{
	ClientWantsToJoinOnServer(SessionResultIndex);
}

void AMenuController::ClientWantsToJoinOnServer_Implementation(int32 SessionResultIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Join server button clicked"));
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
	AMenuGameMode* GameMode = GetWorld()->GetAuthGameMode<AMenuGameMode>();
	if (GameMode)
	{
		GameMode->StartGame();
	}
}
