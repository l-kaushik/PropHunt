// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MenuController.h"
#include "Widget/MenuWidget.h"
#include "Widget/HostWidget.h"
#include "Widget/JoinGameWidget.h"
#include "Widget/LobbyWidget.h"
#include "Widget/PlayerEntryWidget.h"
#include "GameModes/MenuGameMode.h"
#include "GameInstance/PropHuntGameInstance.h"
#include "States/PropHuntPlayerState.h"

#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

AMenuController::AMenuController()
{
	FString BasePath = "/Game/ThirdPerson/Widgets/";

	MenuWidgetBPClassRef = LoadWidgetBlueprint<UMenuWidget>(BasePath + FString("WB_Menu"));
	HostWidgetBPClassRef = LoadWidgetBlueprint<UHostWidget>(BasePath + FString("WB_Host"));
	JoinGameWidgetBPClassRef = LoadWidgetBlueprint<UJoinGameWidget>(BasePath + FString("WB_JoinGame"));
	LobbyWidgetBPClassRef = LoadWidgetBlueprint<ULobbyWidget>(BasePath + FString("WB_Lobby"));
	PlayerEntryWidgetBPClassRef = LoadWidgetBlueprint<UPlayerEntryWidget>(BasePath + FString("WB_PlayerEntry"));
}

void AMenuController::BeginPlay()
{
	// Initialize variables
	PropHuntGameInstance = Cast<UPropHuntGameInstance>(GetWorld()->GetGameInstance());
	PropHuntPlayerState = GetPlayerState<APropHuntPlayerState>();

	if (IsLocalPlayerController()){

		MenuWidgetRef = CreateAndAddWidget<UMenuWidget>(MenuWidgetBPClassRef);
		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);

		// create lobby widget on top of menu widget if its multiplayer game
		if (PropHuntGameInstance->GetIsMultiplayer())
		{
			SetupWidgetForMuliplayer();
		}
	}
}

void AMenuController::CreateHostWidget()
{
	CreateSubWidgetAndHideParent<UHostWidget, UMenuWidget>(HostWidgetBPClassRef, MenuWidgetRef);
}

void AMenuController::CreateJoinWidget()
{
	CreateSubWidgetAndHideParent<UJoinGameWidget, UMenuWidget>(JoinGameWidgetBPClassRef, MenuWidgetRef);
}

void AMenuController::SetupWidgetForMuliplayer()
{
	LobbyWidgetRef = CreateSubWidgetAndHideParent<ULobbyWidget, UMenuWidget>(LobbyWidgetBPClassRef, MenuWidgetRef);
	LobbyWidgetRef->SetIsHost(true);

	if (auto* PlayerEntryWidgetRef = CreateAndValidateWidget<UPlayerEntryWidget>(PlayerEntryWidgetBPClassRef))
	{
		PlayerEntryWidgetRef->SetPlayerNameText(PropHuntPlayerState->GetPlayerName());
		PlayerEntryWidgetRef->SetPingText(FString::SanitizeFloat(PropHuntPlayerState->GetPingInMilliseconds()));

		LobbyWidgetRef->AddPlayerToList(PlayerEntryWidgetRef);
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
