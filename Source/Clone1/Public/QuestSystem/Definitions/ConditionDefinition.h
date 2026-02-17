// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ConditionDefinition.generated.h"

/**
 * 
 */
UCLASS(Abstract,BlueprintType,Blueprintable, EditInlineNew, DefaultToInstanced)
class CLONE1_API UConditionDefinition : public UObject
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditDefaultsOnly)
	FText DisplayText;
	
};
