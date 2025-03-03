// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "MenuController.generated.h"

/**
 * 
 */

class UMenuWidget;
class ULobbyWidget;
class UHostWidget;
class UJoinGameWidget;
class UPlayerEntryWidget;
class UServerEntryWidget;
class UPropHuntGameInstance;
class APropHuntPlayerState;
class APropHuntGameState;
class FOnlineSessionSearchResult;

UCLASS()
class PROPHUNT_API AMenuController : public APlayerController 
{
	GENERATED_BODY()

public:
	AMenuController();

	virtual void BeginPlay() override;

public:
	void ClientWantsToHost(const FName& SessionName,const FString& LevelName, int32 NumPublicConnections, bool IsLANMatch = true);
	void ClientWantsToJoin(int32 SessionResultIndex);
	void HostWantsToStartGame();
	void ClientWantsToQuit();

	void SearchSessions();
	void LoadSessionsInList(const TArray<FOnlineSessionSearchResult>& InSearchResults);;
	void OnPlayerListUpdated(const TArray<APropHuntPlayerState*> &PlayerStates);

public:
	UFUNCTION(NetMulticast, Reliable)
	void HostWantsToStopPlayerListUpdateTimer();

protected:
	UFUNCTION(Server, Reliable)
	void ClientWantsToHostOnServer(const FName& SessionName,const FString& LevelName, int32 NumPublicConnections, bool IsLANMatc);

	UFUNCTION(Server, Reliable)
	void ClientWantsToJoinOnServer(int32 SessionResultIndex);

	UFUNCTION(Server, Reliable)
	void SearchSessionsOnServer();

	UFUNCTION(Server, Reliable)
	void HostWantsToStartGameOnServer();

private:
	void SetupWidgetForMuliplayer();
	void AddNewPlayerToList(const FString& PlayerName, const FString& PingInms);
	void AddServersToList();
	void StartPlayerListUpdateTimer();
	void StopPlayerListUpdateTimer();
	void RefreshPlayerList();

private:
	UMenuWidget* MenuWidgetRef;
	UJoinGameWidget* JoinGameWidgetRef;
	ULobbyWidget* LobbyWidgetRef;

	TSubclassOf<UMenuWidget> MenuWidgetBPClassRef;
	TSubclassOf<UHostWidget> HostWidgetBPClassRef;
	TSubclassOf<UJoinGameWidget> JoinGameWidgetBPClassRef;
	TSubclassOf<ULobbyWidget> LobbyWidgetBPClassRef;
	TSubclassOf<UPlayerEntryWidget> PlayerEntryWidgetBPClassRef;
	TSubclassOf<UServerEntryWidget> ServerEntryWidgetBPClassRef;

	UPropHuntGameInstance* PropHuntGameInstance;
	APropHuntGameState* PropHuntGameState;
	TArray<FOnlineSessionSearchResult> SearchResults;

	bool IsPlayerListUpdateTimerOn;
	FTimerHandle PlayerListUpdateTImer;
};
