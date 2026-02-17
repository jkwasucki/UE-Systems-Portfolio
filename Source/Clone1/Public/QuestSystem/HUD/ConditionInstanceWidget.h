// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ConditionInstanceWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UConditionInstanceWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Description;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CurrentProgress;
};
