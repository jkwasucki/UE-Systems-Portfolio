// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CLONE1_API IInteractable
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	FText GetActionText();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	FText GetActionKeyString();
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Highlight(bool bState);
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	 void Interact(ACharacter* Character);
	
};
