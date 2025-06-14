// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROPHUNT_API ULoadingScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetLoadingMessage(const FString& InMessage = "Loading Map");

private:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* LoadingMessage;
};
