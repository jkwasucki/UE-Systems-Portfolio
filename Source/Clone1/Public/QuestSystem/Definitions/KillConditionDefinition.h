// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "QuestSystem/Definitions/ConditionDefinition.h"
#include "KillConditionDefinition.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UKillConditionDefinition : public UConditionDefinition
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TargetActorTag;
	UPROPERTY(EditDefaultsOnly)
	int32 RequiredCount;
};
