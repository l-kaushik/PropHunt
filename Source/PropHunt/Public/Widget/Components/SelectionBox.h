// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectionBox.generated.h"

class UTextBlock;
class UMasterButton;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectionChanged, const FString&);

UCLASS()
class PROPHUNT_API USelectionBox : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativePreConstruct() override;

	void SetOptions(const TArray<FString>& InOptions);
	void SetActiveOption(int32 OptionIndex);
	void SetActiveOption(const FString& InOption);
	void SetActiveOptionTextOnly(int32 OptionIndex);
	void SetActiveOptionTextOnly(const FString& InOption);

	// event dispatcher
	FOnSelectionChanged OnSelectionChanged;

	void NotifySelectionChanged(const FString& SelectedItem) const;
	int32 GetSelectedOptionIndex() const;
	const FString& GetSelectedOptionString() const;

protected:

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
