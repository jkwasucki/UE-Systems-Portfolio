// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "HorText2Val.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UHorText2Val : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ValueText;
	
	UFUNCTION()
	void SetData(FText Name, FText Val);
	UFUNCTION()
	void UpdateValue(FText Value);
};
