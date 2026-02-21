// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityDatabaseSubsystem.h"

#include "Main/MyGameInstance.h"

void UAbilityDatabaseSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	
	LoadAbilitiesToMemory();
}

void UAbilityDatabaseSubsystem::LoadAbilitiesToMemory()
{

	
	UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (!MyGameInstance) return;

	UAbilityDatabaseAsset* AbilityDatabaseAsset =
		MyGameInstance->AbilityDatabaseAsset.LoadSynchronous();
	
	if (!AbilityDatabaseAsset) return;
	
	for (const TPair <FGameplayTag, TSoftObjectPtr<UAbilityData>>& Pair : AbilityDatabaseAsset->AbilityData)
	{
		UAbilityData* AbilityData = Pair.Value.LoadSynchronous();
		if (AbilityData)
		{
			LoadedAbilities.Add(Pair.Key, AbilityData);
		}
	}
	
}

UAbilityData* UAbilityDatabaseSubsystem::GetAbilityByTag(FGameplayTag Tag)
{
	if (UAbilityData** Found = LoadedAbilities.Find(Tag))
	{
		return *Found;
	}  
	return nullptr;
}
