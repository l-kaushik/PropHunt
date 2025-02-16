// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Macros/WidgetMacros.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIErrorBox.generated.h"

class UImage;
class UOverlay;
class UBorder;
class UVerticalBox;
class UHorizontalBox;
class UScaleBox;
class UMasterButton;
class UMultiLineEditableTextBox;

UCLASS()
class PROPHUNT_API UUIErrorBox : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetMessage(const FString& InMessage);

private:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	void BindClickEvents();
	void OnOkButtonClicked();

private:

	// background overlay
	UPROPERTY(meta = (BindWidget))
	UImage* BackgroundTint;

	// Error box components
	UPROPERTY(meta = (BindWidget))
	UOverlay* ErrorBoxOverlay;

	UPROPERTY(meta = (BindWidget))
	UBorder* ErrorBoxBorder;

	UPROPERTY(meta = (BindWidget))
	UImage* ErrorBoxTint;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ErrorVBox;

	UPROPERTY(meta = (BindWidget))
	UScaleBox* ErrorMessageScaleBox;

	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableTextBox* ErrorMessageTextBox;

	UPROPERTY(meta = (BindWidget))
	UMasterButton* OkButton;
};
