// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/PropHuntGameInstance.h"
#include "Subsystem/PropHuntSubsystem.h"
#include "Controller/MenuController.h"
#include "Utils/PropHuntLog.h"

#include "OnlineSubsystemTypes.h"
#include "OnlineSubsystemUtils.h"
#include "Online/OnlineSessionNames.h"
#include "Interfaces/OnlineSessionInterface.h"

void UPropHuntGameInstance::Init()
{
	Super::Init();

	PropHuntSubsystem = GetSubsystem<UPropHuntSubsystem>();
	bIsMultiplayer = false;
	bIsHost = false;
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
	PropHuntSubsystem->OnFindSessionsCompleteEvent.Clear();
	PropHuntSubsystem->OnFindSessionsCompleteEvent.AddUObject(this, &ThisClass::OnFindSessionsCompleted);
	PropHuntSubsystem->FindSessions(MaxSearchResults, IsLANQuery);
}

void UPropHuntGameInstance::OnFindSessionsCompleted(const TArray<FOnlineSessionSearchResult>& SearchResults, bool Successful)
{
	UE_LOG(LogPropHuntGameInstance, Display, TEXT("OnFindSessionsCompleted: Find session completed"));

	if (SearchResults.IsEmpty())
	{
		UE_LOG(LogPropHuntGameInstance, Warning, TEXT("OnFindSessionsCompleted: Find session result is empty."));
	}

	if (Successful)
	{
		// add to list
		auto* PlayerController = Cast<AMenuController>(GetWorld()->GetFirstPlayerController());

		PlayerController->LoadSessionsInList(SearchResults);
		return;
	}
}

void UPropHuntGameInstance::JoinGameSession(const FName& SessionName, const FOnlineSessionSearchResult& SessionResult)
{
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
