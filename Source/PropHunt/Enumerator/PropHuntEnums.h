#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "PropHuntEnums.generated.h"

UENUM(BlueprintType)
enum class EMenuState : uint8
{
    MainMenu      UMETA(DisplayName = "Main Menu"),
    PlayGameMenu  UMETA(DisplayName = "Play Game Menu"),
    OptionMenu    UMETA(DisplayName = "Qption Menu"),
    HostGameMenu  UMETA(DisplayName = "Host Game Menu"),
    JoinGameMenu  UMETA(DisplayName = "Join Game Menu")
};