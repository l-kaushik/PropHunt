// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/PropHuntGameInstance.h"
#include "Subsystem/PropHuntSubsystem.h"
#include "Controller/MenuController.h"

#include "Interfaces/OnlineSessionInterface.h"

void UPropHuntGameInstance::Init()
{
	Super::Init();

	PropHuntSubsystem = GetSubsystem<UPropHuntSubsystem>();
	bIsMultiplayer = false;
}

bool UPropHuntGameInstance::GetIsMultiplayer()
{
	return bIsMultiplayer;
}

void UPropHuntGameInstance::HostSession(const FName& SessionName, const FString LevelName, int32 NumPublicConnections, bool IsLANMatch)
{
	PropHuntSubsystem->OnCreateSessionCompleteEvent.Clear();
	PropHuntSubsystem->OnCreateSessionCompleteEvent.AddUObject(this, &ThisClass::OnCreateSessionCompleted);
	PropHuntSubsystem->CreateSession(SessionName, LevelName, NumPublicConnections, IsLANMatch);
}

void UPropHuntGameInstance::OnCreateSessionCompleted(bool Successful)
{
	if (Successful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Menu Controller: session created successfully"));

		FString LevelPath("/Game/ThirdPerson/Maps/MenuMap");
		FString CommandString("?listen");

		if (GetWorld()->ServerTravel(LevelPath + CommandString))
		{
			UE_LOG(LogTemp, Warning, TEXT("Menu Controller: Seamless travel succeeded"));
			bIsMultiplayer = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Menu Controller: Failed to seamless travel"));
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
	UE_LOG(LogTemp, Warning, TEXT("Game Instance: find session completed"));

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
			UE_LOG(LogTemp, Warning, TEXT("join Session: Success"));
		break;
		case EOnJoinSessionCompleteResult::SessionIsFull:
			UE_LOG(LogTemp, Warning, TEXT("join Session: session is full"));
		break;
		case EOnJoinSessionCompleteResult::SessionDoesNotExist:
			UE_LOG(LogTemp, Warning, TEXT("join Session: session doesn't exist"));
		break;
		case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
			UE_LOG(LogTemp, Warning, TEXT("join Session: could not retrieve address"));
		break;
		case EOnJoinSessionCompleteResult::AlreadyInSession:
			UE_LOG(LogTemp, Warning, TEXT("join Session: already in session"));
		break;
		case EOnJoinSessionCompleteResult::UnknownError:
			UE_LOG(LogTemp, Warning, TEXT("join Session: unknown error"));
		break;
	default:
		break;
	}
}
