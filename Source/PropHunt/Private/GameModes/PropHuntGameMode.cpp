// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModes/PropHuntGameMode.h"
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
	MyGameState->GetPlayerControllerList().Remove(Cast<APropHuntPlayerController>(ExistingPlayer));
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

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(MyGameState->HunterStartLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	APropCharacter* PropCharacter = World->SpawnActor<APropCharacter>(PropCharBlueprintRef, SpawnTransform, SpawnParams);

	PlayerController->GetPawn()->Destroy();
	PlayerController->Possess(PropCharacter);
}


void APropHuntGameMode::EndTheGame(bool bIsPropWon)
{
	GetWorldTimerManager().ClearTimer(GameLoopTimer);
	MyGameState->SetIsPropWon(bIsPropWon);

	for (auto& PlayerController : MyGameState->GetPlayerControllerList()) {
		if (PlayerController)
		{
			PlayerController->ShowWinScreenWidget(MyGameState->GetIsPropWon());
		}
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		this,
		&APropHuntGameMode::StartNextGame,
		5.0f,
		false
	);
	UE_LOG_BUILD(LogPropHuntGameMode, Display, TEXT("EndTheGame called"));
}

void APropHuntGameMode::StartNextGame()
{
	UE_LOG_BUILD(LogPropHuntGameMode, Display, TEXT("StartNextGame called"));

	GetWorld()->ServerTravel(TEXT("/Game/ThirdPerson/Maps/ThirdPersonMap"));
}

void APropHuntGameMode::CheckGameStarted()
{
	UE_LOG_BUILD(LogPropHuntGameMode, Display, TEXT("CheckGameStarted called"));
	// Start game if we have at least MinPlayerNum in game
	if (!MyGameState->GetHasGameStarted()) {
		int32 ArrayLength = MyGameState->GetPlayerControllerList().Num();
		if (ArrayLength >= MyGameState->GetMinPlayerNum()) {
			StartGameTimer();
		}
	}

	// handle what to do with joined players, when game is already started
}

// wait for "CountdownSeconds" before hunter selection

void APropHuntGameMode::StartGameTimer()
{
	UE_LOG_BUILD(LogPropHuntGameMode, Display, TEXT("StartGameTimer called"));
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
	UE_LOG_BUILD(LogPropHuntGameMode, Display, TEXT("ChooseHunterCharacter called"));
	MyGameState->SetHasGameStarted(true);

	int32 Length = MyGameState->GetPlayerControllerList().Num() - 1;
	int32 RandomIndex = FMath::RandRange(0, Length);
	APropHuntPlayerController* Hunter = *(MyGameState->GetPlayerControllerList().GetData() + RandomIndex);

	if (Hunter) {
		MyGameState->AddHunter(Hunter);
		SpawnHunter();
	}
	else {
		StartGameTimer();	// if hunter not present, reset 
	}

}

void APropHuntGameMode::SpawnHunter()
{
	UE_LOG_BUILD(LogPropHuntGameMode, Display, TEXT("SpawnHunter called"));
	UWorld* World = GetWorld();

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(MyGameState->HunterStartLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	APropHuntCharacter* HunterCharacter = World->SpawnActor<APropHuntCharacter>(CharacterBlueprint, SpawnTransform, SpawnParams);

	/*
	* taking the 0th indexed hunter controller cuz for now only 2 player exits, but when hunter kills more player, more hunter will join, so I have to maintain a counter or something to note that how many hunters have assigned with their pawns
	*/
	APropHuntPlayerController* HunterController = *(MyGameState->GetHunterList().GetData());
	HunterController->GetPawn()->Destroy();		// destroy current actor owned by hunter controller

	// posses the hunter with the spawned hunter character

	if (HunterController) {
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(
			TimerHandle,
			[this, HunterController, HunterCharacter]() {
				if (HasAuthority()) {
					HunterController->Possess(HunterCharacter);
					SetupInitialWidget(HunterController);
					StartGameLoopTimer();
				}
			},
			2,
			false);
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

void APropHuntGameMode::SetupInitialWidget(APropHuntPlayerController* HunterController)
{
	// setup widget for the first hunter
	HunterController->TrySetupPropWidget(false);

	// setup widget for rest of the props
	for (auto& PlayerController : MyGameState->GetPlayerControllerList()) {

		// setup the timer widget
		if (PlayerController)
		{
			PlayerController->StartCountdownWidget();
		}

		// setup the other start screen widget
		if (PlayerController && PlayerController != HunterController) {
			PlayerController->TrySetupPropWidget(true);
		}
	}
}
