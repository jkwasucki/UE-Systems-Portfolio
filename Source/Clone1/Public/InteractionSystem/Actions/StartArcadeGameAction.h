// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionAction.h"
#include "StartArcadeGameAction.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UStartArcadeGameAction : public UInteractionAction
{
	GENERATED_BODY()
public:
	virtual void Execute(UInteractionInstance* Instance) override;
};
