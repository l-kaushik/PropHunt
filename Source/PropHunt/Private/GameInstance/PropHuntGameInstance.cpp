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
