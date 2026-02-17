// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionAction.generated.h"

/**
 * 
 */
class UInteractionInstance;

UCLASS(Abstract)
class CLONE1_API UInteractionAction : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION()
	virtual void Execute(UInteractionInstance* Instance);
};
