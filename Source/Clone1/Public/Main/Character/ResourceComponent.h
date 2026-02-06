// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedByDelta, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnergyChanged, float, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnergyChangedSnapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthChangedSnapshot);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UResourceComponent();

	FOnHealthChanged OnHealthChangedDelegate;
	FOnEnergyChanged OnEnergyChangedDelegate;
	FOnHealthChangedByDelta OnHealthChangedByDeltaDelegate;
	
	//Snapshots / no value
	FOnEnergyChangedSnapshot SnapshotOnEnergyChangedDelegate;	
	FOnHealthChangedSnapshot SnapshotOnHealthChangedDelegate;

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float CurrentHealth = 50;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float CurrentEnergy = 100;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	void UpdateResource(ECharacterResource Resource, float Delta);
	UFUNCTION()
	float UpdateHealth(float Delta);
	UFUNCTION()
	void UpdateEnergy(float Delta);
	UFUNCTION()
	float GetHealth() const;
	UFUNCTION()
	float GetEnergy() const;
};
