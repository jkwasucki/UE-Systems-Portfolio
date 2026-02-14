// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionDefinition.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CLONE1_API IInteractableInterface
{
	GENERATED_BODY()
	
public:
	
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	bool IsInteractedWith();
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Highlight(bool bState);
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	 TSoftObjectPtr<UInteractionDefinition> GetInteractionDefinition();
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	FText GetHUDPrompt();
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void OnInteractionExecuted();
};
