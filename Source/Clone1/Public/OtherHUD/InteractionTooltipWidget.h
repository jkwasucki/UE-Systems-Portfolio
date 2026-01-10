// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionTooltipWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UInteractionTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetDataBP(const FText& KeyText,const  FText& ActionText);
	
};
