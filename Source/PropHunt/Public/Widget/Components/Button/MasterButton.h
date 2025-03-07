// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MasterButton.generated.h"

/**
 * 
 */

class UTextBlock;
class UButton;

DECLARE_MULTICAST_DELEGATE(FOnButtonClicked);

UCLASS()
class PROPHUNT_API UMasterButton : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetLabel(FString NewLabel);
	void SetBackgroundColor(FLinearColor NewColor);

public:

	FOnButtonClicked OnClicked;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Label Properties")
	UTextBlock* ButtonLabel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Button Properties")
	UButton* Button;

protected:
	virtual void NativeConstruct() override;
private:
	bool bEventBound = false;

	UFUNCTION()
	void HandleButtonClicked();
};
