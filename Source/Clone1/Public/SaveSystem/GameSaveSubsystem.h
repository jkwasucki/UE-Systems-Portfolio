// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs/FGameSaveData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameSaveSubsystem.generated.h"

/**
 * 
 */

UCLASS()
class CLONE1_API UGameSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	
protected:
	FName ActiveSlot = "DefaultSlot";
	bool bIsLoadPending = false;
	FGameSaveData PendingLoadData;
	
protected:
	
public:
	UFUNCTION()
	void LoadGame(FName SlotName);
	UFUNCTION()
	void SaveGame(FName SlotName);
	UFUNCTION()
	void ApplyPendingLoad(APawn*  Pawn);
	
};
