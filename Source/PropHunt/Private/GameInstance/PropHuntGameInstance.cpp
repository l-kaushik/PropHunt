// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/PropHuntGameInstance.h"
#include "Subsystem/PropHuntSubsystem.h"

void UPropHuntGameInstance::Init()
{
	Super::Init();

	PropHuntSubsystem = GetSubsystem<UPropHuntSubsystem>();
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
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Menu Controller: Failed to seamless travel"));
	}
}
