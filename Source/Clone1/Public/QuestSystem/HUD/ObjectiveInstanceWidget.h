// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConditionInstanceWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "ObjectiveInstanceWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UObjectiveInstanceWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UConditionInstanceWidget> ConditionInstanceWidgetClass;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ObjectiveNumber;
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* ConditionsList;
	UPROPERTY(meta=(BindWidget))
	UImage* Overlay;

};
