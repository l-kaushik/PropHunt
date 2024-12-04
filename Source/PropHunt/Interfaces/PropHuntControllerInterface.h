// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PropHuntControllerInterface.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UPropHuntControllerInterface : public UInterface
{
	GENERATED_BODY()
};

// actual interface class
class IPropHuntControllerInterface
{
	GENERATED_BODY()

public:

	virtual void SetupPropWidget(bool bIsProp) = 0;
	virtual void UpdateHealthWidget(float NewHealth) = 0;
};




