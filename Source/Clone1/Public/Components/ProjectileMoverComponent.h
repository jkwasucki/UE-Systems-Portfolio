// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/FAbilityTargetData.h"
#include "ProjectileMoverComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UProjectileMoverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProjectileMoverComponent();

protected:

	UPROPERTY()
	bool bCanMove = false;
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void InitializeFromTargetData(const FAbilityTargetData& TargetData);
	void Stop();
private:
	FVector MoveDirection = FVector::ZeroVector;
	float Speed = 2000.f;
	float TraveledDistance = 0.f;
	float MaxRange = 10000.f;
		
};
