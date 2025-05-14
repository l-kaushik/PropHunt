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

struct FMapInfo;
class UMenuWidget;
class ULobbyWidget;
class UHostWidget;
class UJoinGameWidget;
class UPlayerEntryWidget;
class UServerEntryWidget;
class UPropHuntGameInstance;
class APropHuntPlayerState;
class APropHuntGameState;
class AMenuGameMode;
class FOnlineSessionSearchResult;

UCLASS()
class PROPHUNT_API AMenuController : public APlayerController 
{
	GENERATED_BODY()

public:
	AMenuController();

	virtual void BeginPlay() override;
	virtual void ClientReturnToMainMenuWithTextReason_Implementation(const FText& ReturnReason) override;

public:
	void ClientWantsToHost(const FName& SessionName, const FMapInfo& MapInfo, int32 NumPublicConnections, bool IsLANMatch = true);
	void ClientWantsToJoin(int32 SessionResultIndex);
	void UpdateClientReadyStatus(bool IsReady);
	void HostWantsToStartGame();
	void ClientWantsToQuit();
	void HostWantsToQuit();

	void SearchSessions();
	void LoadSessionsInList(const TArray<FOnlineSessionSearchResult>& InSearchResults);;
	void OnPlayerListUpdated(const TArray<APropHuntPlayerState*> &PlayerStates);

	void DisplaySessionError(const FString& ErrorMessage);

public:
	UFUNCTION(NetMulticast, Reliable)
	void HostWantsToStopPlayerListUpdateTimer();

	UFUNCTION(Client, Reliable)
	void ShowLoadingScreen(const FString& InMessage);

	UFUNCTION(Client, Reliable)
	void ClientReceiveMapInfo(const FMapInfo& MapInfo);

protected:
	UFUNCTION(Server, Reliable)
	void ClientWantsToHostOnServer(const FName& SessionName, const FMapInfo& MapInfo, int32 NumPublicConnections, bool IsLANMatc);

	UFUNCTION(Server, Reliable)
	void ClientWantsToJoinOnServer(int32 SessionResultIndex);

	UFUNCTION(Server, Reliable)
	void SearchSessionsOnServer();

	UFUNCTION(Server, Reliable)
	void HostWantsToStartGameOnServer();

	UFUNCTION(Server, Reliable)
	void ServerUpdateReadyStatus(const bool IsReady);

	UFUNCTION(Server, Reliable)
	void ServerRequestMapInfo();

private:
	void InitializeVariable();
	void InitializeMenuWidget();
	void InitializeMultiplayerSettings();
	void SetupDisconnectSettings();

	void SetupWidgetForMuliplayer();
	void AddNewPlayerToList(const APropHuntPlayerState* PlayerState);
	void UpdateStartButtonState(bool IsReady);
	void AddServersToList();
	void StartPlayerListUpdateTimer();
	void StopPlayerListUpdateTimer();
	void RefreshPlayerList();

	void LoadSaveGameData();

private:
	UMenuWidget* MenuWidgetRef;
	ULobbyWidget* LobbyWidgetRef;

	TSubclassOf<UMenuWidget> MenuWidgetBPClassRef;
	TSubclassOf<UHostWidget> HostWidgetBPClassRef;
	TSubclassOf<UJoinGameWidget> JoinGameWidgetBPClassRef;
	TSubclassOf<ULobbyWidget> LobbyWidgetBPClassRef;
	TSubclassOf<UPlayerEntryWidget> PlayerEntryWidgetBPClassRef;
	TSubclassOf<UServerEntryWidget> ServerEntryWidgetBPClassRef;

	UPropHuntGameInstance* PropHuntGameInstance;
	APropHuntGameState* PropHuntGameState;
	TObjectPtr<AMenuGameMode> PropHuntMenuGameMode;
	TArray<FOnlineSessionSearchResult> SearchResults;

	bool IsPlayerListUpdateTimerOn;
	FTimerHandle PlayerListUpdateTImer;

	TSubclassOf<class ULoadingScreenWidget> LoadingScreenWidgetBPClassRef;
	class ULoadingScreenWidget* LoadingWidget;
};
