// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MenuController.h"
#include "Widget/MenuWidget.h"
#include "Widget/HostWidget.h"
#include "Widget/JoinGameWidget.h"
#include "Widget/LobbyWidget.h"
#include "GameModes/MenuGameMode.h"
#include "GameInstance/PropHuntGameInstance.h"

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
}

void AMenuController::BeginPlay()
{
	// store subsystem

	if (HasAuthority())
	{
		PropHuntGameInstance = Cast<UPropHuntGameInstance>(GetWorld()->GetGameInstance());
	}

	if ((HasAuthority() && IsLocalPlayerController()) || !HasAuthority()) {

		MenuWidgetRef = CreateAndAddWidget<UMenuWidget>(MenuWidgetBPClassRef);

		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
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

void AMenuController::ClientWantsToHost(const FName& SessionName, const FString& LevelName, int32 NumPublicConnections, bool IsLANMatch)
{
	ClientWantsToHostOnServer(SessionName, LevelName, NumPublicConnections, IsLANMatch);
}

void AMenuController::ClientWantsToHostOnServer_Implementation(const FName& SessionName, const FString& LevelName, int32 NumPublicConnections, bool IsLANMatch)
{
	PropHuntGameInstance->HostSession(SessionName, LevelName, NumPublicConnections, IsLANMatch);
}
