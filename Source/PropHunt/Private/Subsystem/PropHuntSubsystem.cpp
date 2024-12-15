// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/PropHuntSubsystem.h"

#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"


// initializing *SessionCompleteDelegate, basically adding the callback function to it
// this way of initializing is known as member initializer list
// the order of initialization is depends on the order of declaration in class

UPropHuntSubsystem::UPropHuntSubsystem()
	: CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionCompleted))
	, DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionCompleted))
	, FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsCompleted))
	, JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionCompleted))
	, StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionCompleted))
	, EndSessionCompleteDelegate(FOnEndSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnEndSessionCompleted))
	, UpdateSessionCompleteDelegate(FOnUpdateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnUpdateSessionCompleted))

{
}

void UPropHuntSubsystem::CreateSession(const FName& SessionName,const FString& LevelName, int32 NumPublicConnections, bool IsLANMatch)
{
	// return and broadcast false if session interface is not found
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnCreateSessionCompleteEvent.Broadcast(false);
		return;
	}

	// session related settings
	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->NumPrivateConnections = 0;
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowInvites = true;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
	LastSessionSettings->bIsDedicated = false;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bIsLANMatch = IsLANMatch;
	LastSessionSettings->bShouldAdvertise = true;

	// set map name
	LastSessionSettings->Set(SETTING_MAPNAME, LevelName, EOnlineDataAdvertisementType::ViaOnlineService);
	// set display name
	LastSessionSettings->Set(TEXT("SESSION_DISPLAY_NAME"), SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);

	// adding delegate to on complete and storing handle as well
	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	// retrives the local player object, attempts to create the session with specified settings
	// if fails, clears the delegate and broadcast false
	// set a custom session name, instead of using NAME_GameSession
	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), SessionName, *LastSessionSettings))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		OnCreateSessionCompleteEvent.Broadcast(false);
	}
}

// get called when the session creation process completes
// broadcast the sucess or failure
void UPropHuntSubsystem::OnCreateSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	OnCreateSessionCompleteEvent.Broadcast(Successful);

}

void UPropHuntSubsystem::DestroySession(const FName& SessionName)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnDestroySessionCompleteEvent.Broadcast(false);
		return;
	}

	DestroySessionCompleteDelegateHandle =
		SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	if (!SessionInterface->DestroySession(SessionName))
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);

		OnDestroySessionCompleteEvent.Broadcast(false);
	}
}

FNamedOnlineSession* UPropHuntSubsystem::FindSessionByName(FName SessionName)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		return nullptr;
	}

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SessionName);
	return ExistingSession;
}

void UPropHuntSubsystem::OnDestroySessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}

	OnDestroySessionCompleteEvent.Broadcast(Successful);
}

void UPropHuntSubsystem::FindSessions(int32 MaxSearchResults, bool IsLANQuery)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IsLANQuery;
	
	// this line makes sure that server hosted by a player are visible in search result rather than dedicated one.
	// another thing is there is no need for player to be on same wifi, as it uses steam or xbox live, to show world's from friend list.
	LastSessionSearch->QuerySettings.Set(TEXT("PRESENCESEARCH"), true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}

void UPropHuntSubsystem::OnFindSessionsCompleted(bool Successful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), Successful);
		return;
	}

	OnFindSessionsCompleteEvent.Broadcast(LastSessionSearch->SearchResults, Successful);
}

void UPropHuntSubsystem::JoinSession(const FName& SessionName, const FOnlineSessionSearchResult& SessionResult)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnJoinSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegateHandle =
		SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->JoinSession(*localPlayer->GetPreferredUniqueNetId(), SessionName, SessionResult))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		OnJoinSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UPropHuntSubsystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		if (Result == EOnJoinSessionCompleteResult::Type::Success)
		{
			TryTravelToCurrentSession(SessionName);
		}
	}

	OnJoinSessionCompleteEvent.Broadcast(Result);
}

bool UPropHuntSubsystem::TryTravelToCurrentSession(const FName& SessionName)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		return false;
	}

	FString connectString;
	if (!SessionInterface->GetResolvedConnectString(SessionName, connectString))
	{
		return false;
	}

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	playerController->ClientTravel(connectString, TRAVEL_Absolute);
	return true;
}

void UPropHuntSubsystem::StartSession(const FName& SessionName)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnStartSessionCompleteEvent.Broadcast(false);
		return;
	}

	StartSessionCompleteDelegateHandle =
		SessionInterface->AddOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegate);

	if (!SessionInterface->StartSession(SessionName))
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);

		OnStartSessionCompleteEvent.Broadcast(false);
	}
}

void UPropHuntSubsystem::OnStartSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
	}

	OnStartSessionCompleteEvent.Broadcast(Successful);
}

void UPropHuntSubsystem::EndSession(const FName& SessionName)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnEndSessionCompleteEvent.Broadcast(false);
		return;
	}

	EndSessionCompleteDelegateHandle =
		SessionInterface->AddOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegate);

	if (!SessionInterface->EndSession(SessionName))
	{
		SessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);

		OnEndSessionCompleteEvent.Broadcast(false);
	}
}

void UPropHuntSubsystem::OnEndSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());;
	if (SessionInterface)
	{
		SessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);
	}

	OnEndSessionCompleteEvent.Broadcast(Successful);
}

void UPropHuntSubsystem::UpdateSession(const FName& SessionName, const FString& NewLevelName)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnUpdateSessionCompleteEvent.Broadcast(false);
		return;
	}

	TSharedPtr<FOnlineSessionSettings> updatedSessionSettings = MakeShareable(new FOnlineSessionSettings(*LastSessionSettings));
	updatedSessionSettings->Set(SETTING_MAPNAME, NewLevelName, EOnlineDataAdvertisementType::ViaOnlineService);

	UpdateSessionCompleteDelegateHandle =
		SessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegate);

	if (!SessionInterface->UpdateSession(SessionName, *updatedSessionSettings))
	{
		SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);

		OnUpdateSessionCompleteEvent.Broadcast(false);
	}
	else
	{
		LastSessionSettings = updatedSessionSettings;
	}
}

void UPropHuntSubsystem::OnUpdateSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);
	}

	OnUpdateSessionCompleteEvent.Broadcast(Successful);
}