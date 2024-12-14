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
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidgetBPClass(TEXT("/Game/ThirdPerson/Widgets/WB_Menu"));
	if (MenuWidgetBPClass.Succeeded())
	{
		MenuWidgetBPClassRef = MenuWidgetBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UHostWidget> HostWidgetBPClass(TEXT("/Game/ThirdPerson/Widgets/WB_Host"));
	if (HostWidgetBPClass.Succeeded())
	{
		HostWidgetBPClassRef = HostWidgetBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UJoinGameWidget> JoinGameWidgetBPClass(TEXT("/Game/ThirdPerson/Widgets/WB_JoinGame"));
	if (JoinGameWidgetBPClass.Succeeded())
	{
		JoinGameWidgetBPClassRef = JoinGameWidgetBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<ULobbyWidget> LobbyWidgetBPClass(TEXT("/Game/ThirdPerson/Widgets/WB_Lobby"));
	if (LobbyWidgetBPClass.Succeeded())
	{
		LobbyWidgetBPClassRef = LobbyWidgetBPClass.Class;
	}
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
