// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Structs/FGameSaveData.h"
#include "SaveGameObject.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API USaveGameObject : public USaveGame
{
	GENERATED_BODY()
public:	
	UPROPERTY()
	FGameSaveData SaveData;
	
};
