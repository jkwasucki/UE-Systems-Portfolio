// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Engine/DataAsset.h"
#include "InteractionSystem/Actions/InteractionAction.h"
#include "InteractionDefinition.generated.h"

/**
 * 
 */

UCLASS(BlueprintType,Blueprintable)
class CLONE1_API UInteractionDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Interaction")
	TSoftObjectPtr<UInputAction> InputAction;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Interaction")
	FText Prompt;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Interaction")
	float Duration = 1.f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Interaction")
	TArray<TSubclassOf<UInteractionAction>> Actions;
	
	// Optional: Sound
	// Optional: Visuals
};
