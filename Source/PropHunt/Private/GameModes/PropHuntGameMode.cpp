// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModes/PropHuntGameMode.h"
#include "Utils/MapManager.h"
#include "Characters/PropCharacter.h"
#include "Characters/PropHuntCharacter.h"
#include "Controller/PropHuntPlayerController.h"
#include "States/PropHuntGameState.h"
#include "States/PropHuntPlayerState.h"
#include "GameInstance/PropHuntGameInstance.h"
#include "Utils/PropHuntLog.h"

#include "UObject/ConstructorHelpers.h"

APropHuntGameMode::APropHuntGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_PropCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		PlayerControllerClass = APropHuntPlayerController::StaticClass();
		GameStateClass = APropHuntGameState::StaticClass();
		PlayerStateClass = APropHuntPlayerState::StaticClass();
	}

	// getting reference to prop hunt character blueprint class
	CharacterBlueprint = LoadBlueprint<APropHuntCharacter>(FString("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));

	PropCharBlueprintRef = LoadBlueprint<APropCharacter>(FString("/Game/ThirdPerson/Blueprints/BP_PropCharacter"));

	bUseSeamlessTravel = true;
}

void APropHuntGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto* PlayerController = Cast<APropHuntPlayerController>(NewPlayer);
	
	SpawnPlayer(PlayerController);
	MyGameState->AddPlayerController(PlayerController);
	CheckGameStarted();
}

void APropHuntGameMode::Logout(AController* ExistingPlayer)
{
	Super::Logout(ExistingPlayer);
	MyGameState->RemovePlayerController(Cast<APropHuntPlayerController>(ExistingPlayer));
}

void APropHuntGameMode::InitGameState()
{
	Super::InitGameState();

	MyGameState = GetGameState<APropHuntGameState>();
	if (MyGameState)
	{
		MyGameState->SetMinPlayerNum(GetGameInstance<UPropHuntGameInstance>()->GetPlayerNum());
	}
}

void APropHuntGameMode::SpawnPlayer(APropHuntPlayerController* PlayerController)
{
	if (!PlayerController) return;
	UWorld* World = GetWorld();

	AActor* SpawnPoint = FindPlayerStart(nullptr);
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GetGameInstance<UPropHuntGameInstance>()->GetMapInfo().PropSpawnCoordinate);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	APropCharacter* PropCharacter = World->SpawnActor<APropCharacter>(PropCharBlueprintRef, SpawnTransform, SpawnParams);

	PlayerController->GetPawn()->Destroy();
	PlayerController->Possess(PropCharacter);
}


void APropHuntGameMode::EndTheGame(bool bIsPropWon)
{

	/*
	* change the current win screen to a leader board kind of system, that shows player kill count, person with most hidden time, and then give the host permission to start the game or quit the game.
	*/

	GetWorldTimerManager().ClearTimer(GameLoopTimer);
	MyGameState->SetIsPropWon(bIsPropWon);

	for (auto& PlayerController : MyGameState->GetPlayerControllerList()) {
		if (PlayerController)
		{
			PlayerController->ShowWinScreenWidget();
		}
	}
	UE_LOG_NON_SHIP(LogPropHuntGameMode, Display, TEXT("EndTheGame called"));
}

void APropHuntGameMode::StartNextGame()
{
	UE_LOG_NON_SHIP(LogPropHuntGameMode, Display, TEXT("StartNextGame called"));

	GetWorld()->ServerTravel(MapManager::GetMapAddressWithListen(GetGameInstance<UPropHuntGameInstance>()->GetMapInfo().Name));
}

/* check if number of players is equal to number of players in hub, start/reset the timer after the last joined player basically fail safe when any 1 or more player disconnect between level transition.*/
void APropHuntGameMode::CheckGameStarted()
{
	UE_LOG_NON_SHIP(LogPropHuntGameMode, Display, TEXT("CheckGameStarted called"));
	// Start game if we have at least MinPlayerNum in game
	if (!MyGameState->GetHasGameStarted()) {
		int32 ArrayLength = MyGameState->GetPlayerControllerList().Num();
		if (ArrayLength >= MyGameState->GetMinPlayerNum() /*|| 30 seconds of last player joined*/) {
			StartGameTimer();
		}
	}

	// handle what to do with joined players, when game is already started
}

// wait for "CountdownSeconds" before hunter selection

void APropHuntGameMode::StartGameTimer()
{
	UE_LOG_NON_SHIP(LogPropHuntGameMode, Display, TEXT("StartGameTimer called"));
	float CountdownSeconds = 5.0f;
	FTimerHandle StartGameCountdownHandler;

	GetWorldTimerManager().SetTimer(
		StartGameCountdownHandler, 
		this,
		&APropHuntGameMode::ChooseHunterCharacter, 
		CountdownSeconds, 
		false);
}

void APropHuntGameMode::ChooseHunterCharacter()
{
	UE_LOG_NON_SHIP(LogPropHuntGameMode, Display, TEXT("ChooseHunterCharacter called"));

	int32 Length = MyGameState->GetPlayerControllerList().Num() - 1;
	int32 RandomIndex = FMath::RandRange(0, Length);
	APropHuntPlayerController* Hunter = *(MyGameState->GetPlayerControllerList().GetData() + RandomIndex);

	if (Hunter) {
		SpawnHunter(Hunter);
		MyGameState->SetHasGameStarted(true);
		// after spawning hunter, show message on every prop that hunter will spawn in 10 seconds hide

		// spawn hunter to prop location after x seconds
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(
			TimerHandle,
			[this, Hunter]() {
				Hunter->GetCharacter()->TeleportTo(GetGameInstance<UPropHuntGameInstance>()->GetMapInfo().PropSpawnCoordinate, FRotator(), false, false);

				SetupInitialWidget();
				StartGameLoopTimer();
			},
			10,
			false
		);
	}
	else {
		StartGameTimer();	// if hunter not present, reset 
	}

}

APropHuntCharacter* APropHuntGameMode::SpawnCharacter()
{
	UWorld* World = GetWorld();

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GetGameInstance<UPropHuntGameInstance>()->GetMapInfo().HunterSpawnCoordinate);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	APropHuntCharacter* HunterCharacter = World->SpawnActor<APropHuntCharacter>(CharacterBlueprint, SpawnTransform, SpawnParams);

	return HunterCharacter;
}

void APropHuntGameMode::SpawnHunter(APropHuntPlayerController* HunterController)
{
	UE_LOG_NON_SHIP(LogPropHuntGameMode, Display, TEXT("SpawnHunter called"));

	MyGameState->AddHunter(HunterController);
	APropHuntCharacter* HunterCharacter = SpawnCharacter();
	HunterController->GetPawn()->Destroy();		// destroy current actor owned by hunter controller

	// posses the hunter with the spawned hunter character

	if (HunterController) {
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(
			TimerHandle,
			[this, HunterController, HunterCharacter]() {
				if (HasAuthority()) {
					HunterController->Possess(HunterCharacter);

					/*
					* make sure here, only the HunterController's UI get updated, and setup initial widget for props somewhere else
					* it breaks the SRP
					*/

					HunterController->TrySetupPropWidget(false);
				}
			},
			2,
			false
		);
	}
}

void APropHuntGameMode::StartGameLoopTimer()
{
	GetWorldTimerManager().SetTimer(
		GameLoopTimer,
		this,
		&APropHuntGameMode::TimerFinishEndGame,
		APropHuntGameState::GAME_TIME_IN_SECONDS,
		false);
}

void APropHuntGameMode::TimerFinishEndGame()
{
	EndTheGame(true);
}

void APropHuntGameMode::SetupInitialWidget()
{
	// since running for first time and only 1 hunter will exist that time

	APropHuntPlayerController* HunterController = MyGameState->GetHunterList()[0];

	// setup widget for rest of the props
	for (auto& PlayerController : MyGameState->GetPlayerControllerList()) {

		// setup the timer widget
		if (PlayerController)
		{
			PlayerController->StartCountdownWidget();
		}

		// setup the other start screen widget
		if (PlayerController != HunterController) {
			PlayerController->TrySetupPropWidget(true);
		}
	}
}

void APropHuntGameMode::HandlePropDeath(APropHuntPlayerController* PlayerController)
{
	int HunterCount = MyGameState->GetHunterList().Num();
	int TotalPlayerCount = MyGameState->GetPlayerControllerList().Num();
	int PropCount = TotalPlayerCount - HunterCount - 1; // this 1 is for the current prop who is dead but still not hunter

	UE_LOG_NON_SHIP(LogPropHuntGameMode, Warning, TEXT("HunterCount: %d, PlayerCount: %d, PropCount: %d"), HunterCount, TotalPlayerCount, PropCount);

	// delaying the process of showing end screen, so last kill gets registered
	if (PropCount <= 0)
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(
			TimerHandle,
			[this]() {
				this->EndTheGame(false);
			},
			2.0f,
			false
		);
	}

	PlayerController->SetIsProp(false);
	SpawnHunter(PlayerController);

}

void APropHuntGameMode::CleanupPlayerExitFromScoreboard()
{
	int32 NewMinPlayerNum = MyGameState->GetMinPlayerNum() - 1;
	MyGameState->SetMinPlayerNum(NewMinPlayerNum);
	GetGameInstance<UPropHuntGameInstance>()->SetPlayerNum(NewMinPlayerNum);
}
