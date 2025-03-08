// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/PropHuntGameInstance.h"
#include "GameModes/MenuGameMode.h"
#include "Subsystem/PropHuntSubsystem.h"
#include "Controller/MenuController.h"
#include "Utils/PropHuntLog.h"

#include "OnlineSubsystemTypes.h"
#include "OnlineSubsystemUtils.h"
#include "Online/OnlineSessionNames.h"
#include "Interfaces/OnlineSessionInterface.h"

#define GET_CALLEE_INFO() \
    FString::Printf(TEXT("%s"), *FString(ANSI_TO_TCHAR(__FUNCTION__)))

void UPropHuntGameInstance::Init()
{
	Super::Init();

	PropHuntSubsystem = GetSubsystem<UPropHuntSubsystem>();
	bIsMultiplayer = false;
	bIsHost = false;
}

bool UPropHuntGameInstance::IsCurrentSessionName(const FString& CalleInfo)
{
	if (CurrentSessionName.IsNone())
	{
		// TODO: show error window
		UE_LOG(LogPropHuntGameInstance, Warning, TEXT("CurrentSessionName is none in %s"), *CalleInfo);

		return true;
	}

	return false;
}

AMenuController* UPropHuntGameInstance::GetPlayerController()
{
	static auto* PlayerController = Cast<AMenuController>(GetWorld()->GetFirstPlayerController());
	return PlayerController;
}

void UPropHuntGameInstance::SetPlayerNum(int32 InPlayerNum)
{
	PlayerNum = InPlayerNum;
}

int32 UPropHuntGameInstance::GetPlayerNum() const
{
	return PlayerNum;
}

bool UPropHuntGameInstance::GetIsMultiplayer() const
{
	return bIsMultiplayer;
}

bool UPropHuntGameInstance::GetIsHost() const
{
	return bIsHost;
}

void UPropHuntGameInstance::HostSession(const FName& SessionName, const FString LevelName, int32 NumPublicConnections, bool IsLANMatch)
{
	CurrentSessionName = SessionName;
	PropHuntSubsystem->OnCreateSessionCompleteEvent.Clear();
	PropHuntSubsystem->OnCreateSessionCompleteEvent.AddUObject(this, &ThisClass::OnCreateSessionCompleted);
	PropHuntSubsystem->CreateSession(SessionName, LevelName, NumPublicConnections, IsLANMatch);	
}

void UPropHuntGameInstance::OnCreateSessionCompleted(bool Successful)
{
	if (Successful)
	{
		UE_LOG(LogPropHuntGameInstance, Display, TEXT("OnCreateSessionCompleted: session created successfully"));

		FString LevelPath("/Game/ThirdPerson/Maps/MenuMap");
		FString CommandString("?listen");

		if (GetWorld()->ServerTravel(LevelPath + CommandString))
		{
			UE_LOG(LogPropHuntGameInstance, Display, TEXT("OnCreateSessionCompleted: Seamless travel succeeded"));
			bIsMultiplayer = true;
			bIsHost = true;
		}
	}
	else
	{
		UE_LOG(LogPropHuntGameInstance, Error, TEXT("OnCreateSessionCompleted: Failed to create session"));
	}
}

void UPropHuntGameInstance::FindSessions(int32 MaxSearchResults, bool IsLANQuery)
{
	FindSessionSettings.MaxSearchResults = MaxSearchResults;
	FindSessionSettings.bIsLanQuery = IsLANQuery;

	StartFindSessionLoop();
}

void UPropHuntGameInstance::StartFindSessions()
{
	if (FindSessionSettings.bIsFindingSession) {
		UE_LOG(LogPropHuntGameInstance, Warning, TEXT("In StartFindSessions bIsFindingSession is true"));
		return;
	}
	FindSessionSettings.bIsFindingSession = true;
	UE_LOG(LogPropHuntGameInstance, Warning, TEXT("In StartFindSessions sent a search request"));

	PropHuntSubsystem->OnFindSessionsCompleteEvent.Clear();
	PropHuntSubsystem->OnFindSessionsCompleteEvent.AddUObject(this, &ThisClass::OnFindSessionsCompleted);
	PropHuntSubsystem->FindSessions(FindSessionSettings.MaxSearchResults, FindSessionSettings.bIsLanQuery);
}

void UPropHuntGameInstance::OnFindSessionsCompleted(const TArray<FOnlineSessionSearchResult>& SearchResults, bool Successful)
{
	UE_LOG(LogPropHuntGameInstance, Display, TEXT("OnFindSessionsCompleted: Find session completed"));

	if (SearchResults.IsEmpty())
	{
		UE_LOG(LogPropHuntGameInstance, Warning, TEXT("OnFindSessionsCompleted: Find session result is empty."));

		StopFindSessionLoop();
	}

	if (Successful)
	{
		// add to list
		auto* PlayerController = Cast<AMenuController>(GetWorld()->GetFirstPlayerController());

		PlayerController->LoadSessionsInList(SearchResults);
	}

	// keep fetching udpated session informations
	// TODO: stop searching if player closes the widget
	FindSessionSettings.bIsFindingSession = false;
}

void UPropHuntGameInstance::JoinGameSession(const FName& SessionName, const FOnlineSessionSearchResult& SessionResult)
{
	// stop fetching new session information
	StopFindSessionLoop();

	CurrentSessionName = SessionName;
	PropHuntSubsystem->OnJoinSessionCompleteEvent.Clear();
	PropHuntSubsystem->OnJoinSessionCompleteEvent.AddUObject(this, &ThisClass::OnJoinSessionCompleted);
	PropHuntSubsystem->JoinSession(SessionName, SessionResult);
}

void UPropHuntGameInstance::OnJoinSessionCompleted(EOnJoinSessionCompleteResult::Type Result)
{
	switch (Result)
	{
	case EOnJoinSessionCompleteResult::Success:
			UE_LOG(LogPropHuntGameInstance, Display, TEXT("OnJoinSessionCompleted: Success"));
			bIsMultiplayer = true;
			PropHuntSubsystem->TryTravelToSession(CurrentSessionName);
		break;
		case EOnJoinSessionCompleteResult::SessionIsFull:
			UE_LOG(LogPropHuntGameInstance, Warning, TEXT("OnJoinSessionCompleted: session is full"));
		break;
		case EOnJoinSessionCompleteResult::SessionDoesNotExist:
			UE_LOG(LogPropHuntGameInstance, Warning, TEXT("OnJoinSessionCompleted: session doesn't exist"));
		break;
		case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
			UE_LOG(LogPropHuntGameInstance, Warning, TEXT("OnJoinSessionCompleted: could not retrieve address"));
		break;
		case EOnJoinSessionCompleteResult::AlreadyInSession:
			UE_LOG(LogPropHuntGameInstance, Warning, TEXT("OnJoinSessionCompleted: already in session"));
		break;
		case EOnJoinSessionCompleteResult::UnknownError:
			UE_LOG(LogPropHuntGameInstance, Warning, TEXT("OnJoinSessionCompleted: unknown error"));
		break;
	default:
		break;
	}
}

void UPropHuntGameInstance::StartSession()
{
	PropHuntSubsystem->OnStartSessionCompleteEvent.Clear();
	PropHuntSubsystem->OnStartSessionCompleteEvent.AddUObject(this, &ThisClass::OnStartSessionCompleted);
	PropHuntSubsystem->StartSession(CurrentSessionName);
}

void UPropHuntGameInstance::OnStartSessionCompleted(bool Successful)
{
	if (!Successful)
	{
		// display proper error to host
		UE_LOG(LogPropHuntGameInstance, Error, TEXT("Failed to start the session"));
		return;
	}

	auto* PlayerController = Cast<AMenuController>(GetWorld()->GetFirstPlayerController());
	PlayerController->HostWantsToStopPlayerListUpdateTimer();

	auto* GameMode = GetWorld()->GetAuthGameMode<AMenuGameMode>();
	if (GameMode)
	{
		GameMode->StartGame();
	}
}

void UPropHuntGameInstance::RegisterPlayer(const FUniqueNetId& PlayerId)
{
	// Prevent registering if there is no session present
	if (IsCurrentSessionName(GET_CALLEE_INFO())) return;

	PropHuntSubsystem->OnRegisterPlayerCompleteEvent.Clear();
	PropHuntSubsystem->OnRegisterPlayerCompleteEvent.AddUObject(this, &ThisClass::OnRegisterPlayerCompleted);
	PropHuntSubsystem->RegisterPlayer(CurrentSessionName, PlayerId);
}

void UPropHuntGameInstance::OnRegisterPlayerCompleted(bool Successful)
{
	// TODO: display proper error
	// Handle edge cases like: if player couldn't register but joined the world?
	if(!Successful)
		UE_LOG(LogPropHuntGameInstance, Error, TEXT("Failed to register player"));
}

void UPropHuntGameInstance::UnregisterPlayer(const FUniqueNetId& PlayerId)
{
	PropHuntSubsystem->OnUnregisterPlayerCompleteEvent.Clear();
	PropHuntSubsystem->OnUnregisterPlayerCompleteEvent.AddUObject(this, &ThisClass::OnUnregisterPlayerCompleted);
	PropHuntSubsystem->UnregisterPlayer(CurrentSessionName, PlayerId);
}

void UPropHuntGameInstance::OnUnregisterPlayerCompleted(bool Successful)
{
	// TODO: display proper error

	if (Successful)
	{
		UE_LOG(LogPropHuntGameInstance, Warning, TEXT("Player unregistered success"));
		return;
	}

	UE_LOG(LogPropHuntGameInstance, Error, TEXT("Failed to unregister player"));
}

void UPropHuntGameInstance::StartFindSessionLoop()
{
	StopFindSessionLoop();
	GetWorld()->GetTimerManager().SetTimer(FindSessionTimerHandle, this, &ThisClass::StartFindSessions, 3.0f, true);
}

void UPropHuntGameInstance::StopFindSessionLoop()
{
	GetWorld()->GetTimerManager().ClearTimer(FindSessionTimerHandle);
}

void UPropHuntGameInstance::DestroySession()
{
	PropHuntSubsystem->OnDestroySessionCompleteEvent.Clear();
	PropHuntSubsystem->OnDestroySessionCompleteEvent.AddUObject(this, &ThisClass::onDestroySessionCompleted);
	PropHuntSubsystem->DestroySession(CurrentSessionName);
}

void UPropHuntGameInstance::onDestroySessionCompleted(bool Successful)
{
	if (Successful)
	{
		UE_LOG_NON_SHIP(LogPropHuntGameInstance, Warning, TEXT("Destroyed session compelted"));
	}
}

void UPropHuntGameInstance::QuitGameCleanup()
{
	bIsMultiplayer = false;
	DestroySession();
}