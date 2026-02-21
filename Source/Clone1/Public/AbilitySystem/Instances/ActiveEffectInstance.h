// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs/FCharacterEffect.h"
#include "ActiveEffectInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectEnd, UActiveEffectInstance*, EffectInstance);
/**
 * 
 */
class ABaseCharacter;
class UAttributesComponent;
UCLASS()
class CLONE1_API UActiveEffectInstance : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	FTimerHandle OvertimeTick;
	UPROPERTY()
	FTimerHandle DurationTick;
	UPROPERTY()
	float CachedOvertimeTickValue = 0.f;
	UPROPERTY()
	int32 TotalOvertimeTicks = 0;
	UPROPERTY()
	int32 ExecutedOvertimeTicks = 0;
	UPROPERTY()
	float AppliedRawValue = 0;
public:
	UPROPERTY()
	TWeakObjectPtr<ABaseCharacter> CharacterInstance;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FGuid EffectInstanceID;								// Runtime instance id of this effect
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FGuid SourceInstanceID;								// Does this effect come from some other instanced object it has to keep reference of? (eg. Ability) 
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FCharacterEffect CharacterEffectDefinition;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AActor* EffectOrigin;								// From which actor the effect took place
	
	FOnEffectEnd OnEffectEndDelegate;
	
	UFUNCTION()
	void Apply();
	
	void Initialize(AActor* InEffectOrigin,ABaseCharacter* EffectTarget, FCharacterEffect& CharacterEffect, FGuid& SourceInstanceID);
	UFUNCTION()
	void SetTimers();
	UFUNCTION()
	void RemoveEffect(bool bFailure);
	UFUNCTION()
	void RemoveEffect_Timer();
	UFUNCTION()
	void ApplyOvertimeTick(float Value);
	UFUNCTION()
	void Reset();
	UFUNCTION()
	void Revert();
	UFUNCTION()
	void HandleOvertimeTick();
	
};
