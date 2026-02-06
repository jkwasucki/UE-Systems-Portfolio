// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ActiveAbilityInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnCastFinished);
/**
 * 
 */
UCLASS()
class CLONE1_API UActiveAbilityInstance : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag AbilityTag;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGuid AbilityInstanceID;
	
	FOnCastFinished  OnCastFinishedDelegate;
	
	void StartCast(float Duration);
	void CancelCast();
private:
	FTimerHandle CastTimerHandle;
};
