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

	void CreateHostWidget();
	void CreateJoinWidget();

	void SearchSessions();
	void LoadSessionsInList(const TArray<FOnlineSessionSearchResult>& InSearchResults);

protected:
	UFUNCTION(Server, Reliable)
	void ClientWantsToHostOnServer(const FName& SessionName,const FString& LevelName, int32 NumPublicConnections, bool IsLANMatc);

	UFUNCTION(Server, Reliable)
	void ClientWantsToJoinOnServer(int32 SessionResultIndex);

	UFUNCTION(Server, Reliable)
	void SearchSessionsOnServer();

private:
	void SetupWidgetForMuliplayer();

private:
	template<typename T>
	TSubclassOf<T> LoadWidgetBlueprint(const FString WidgetPath)
	{
		static ConstructorHelpers::FClassFinder<T> WidgetBPClass(*WidgetPath);
		return WidgetBPClass.Succeeded() ? WidgetBPClass.Class : nullptr;
	}

	template<typename T>
	T* CreateAndValidateWidget(UClass* WidgetBPClassRef)
	{
		if (!WidgetBPClassRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("WidgetBPClassRef is invalid"));
			return nullptr;
		}

		T* WidgetRef = CreateWidget<T>(this, WidgetBPClassRef);

		if (!WidgetRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create widget, check template argument type and WidgetBPClassRef"));
			return nullptr;
		}

		return WidgetRef;
	}

	template<typename T>
	T* CreateAndAddWidget(UClass* WidgetBPClassRef)
	{
		T* WidgetRef = CreateAndValidateWidget<T>(WidgetBPClassRef);
		WidgetRef->AddToViewport();
		return WidgetRef;
	}

	template<typename T, typename U>
	T* CreateSubWidgetAndHideParent(UClass* WidgetBPClassRef, U* ParentWidget)
	{
		T* WidgetRef = CreateAndAddWidget<T>(WidgetBPClassRef);

		if (WidgetRef)
		{
			WidgetRef->SetParentWidget(ParentWidget);
			ParentWidget->SetVisibility(ESlateVisibility::Collapsed);
		}

		return WidgetRef;
	}

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
	APropHuntPlayerState* PropHuntPlayerState;
	TArray<FOnlineSessionSearchResult> SearchResults;
};
