// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/CharacterTypes.h"
#include "CharacterMoverComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UCharacterMoverComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	bool bIsMoving = false;
public:	
	// Sets default values for this component's properties
	UCharacterMoverComponent();
	
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float BaseWalkSpeed = 600.f;
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float WeightStrengthMultiplier = 4.0f;
	
	
	float WeightSpeedMultiplier = 1.0;
	float AttributeSpeedMultiplier = 1.0f;
	
	FOnStateRequest OnStateRequestDelegate;
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION()
	void ToggleMovement(bool bState);
	UFUNCTION()
	void UpdateSpeed();
	UFUNCTION()
	void ModifySpeedByWeight(int32 CurrentWeight, int32 MaxWeight);
	UFUNCTION()
	bool IsCharacterMoving();
};
