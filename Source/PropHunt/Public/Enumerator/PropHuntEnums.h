#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "PropHuntEnums.generated.h"

UENUM(BlueprintType)
enum class EMenuState : uint8
{
    MainMenu      UMETA(DisplayName = "Main Menu"),
    PlayGameMenu  UMETA(DisplayName = "Play Game Menu"),
    OptionMenu    UMETA(DisplayName = "Option Menu"),
    HelpMenu      UMETA(DisplayName = "How To Play Menu"),
    ProfileMenu   UMETA(DisplayNAme = "Profile Menu")
};