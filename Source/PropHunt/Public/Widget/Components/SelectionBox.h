// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectionBox.generated.h"

class UTextBlock;
class UMasterButton;

DECLARE_MULTICAST_DELEGATE(FOnSelectionChanged);

UCLASS()
class PROPHUNT_API USelectionBox : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativePreConstruct() override;

	void SetOptions(const TArray<FString>& InOptions);

	// event dispatcher
	FOnSelectionChanged OnSelectionChanged;

	void NotifySelectionChanged();
protected:
	void SetSelectOptions(const FString& InOption);

	UFUNCTION()
	void OnLeftArrowButtonClicked();

	UFUNCTION()
	void OnRightArrowButtonClicked();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection", meta = (ExposeOnSpawn = "true"))
	TArray<FString> SelectableOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection", meta = (ExposeOnSpawn = "true"))
	int32 CurrentOption = 0;
	
private:
	UPROPERTY(meta = (BindWidget))
	UMasterButton* LeftArrowButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SelectionText;
	
	UPROPERTY(meta = (BindWidget))
	UMasterButton* RightArrowButton;
};
