// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Definitions/AbilityData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AbilityDatabaseSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UAbilityDatabaseSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
protected:	
	UPROPERTY()
	TMap<FGameplayTag, UAbilityData*> LoadedAbilities;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	UFUNCTION()
	void LoadAbilitiesToMemory();
public:

	UFUNCTION()
	UAbilityData* GetAbilityByTag(FGameplayTag Tag);
};
