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
	UPROPERTY(ReplicatedUsing=OnRep_Health)
	float CurrentHealth = 100;
	UPROPERTY(ReplicatedUsing=OnRep_Energy)
	float CurrentEnergy = 100;
	
	UFUNCTION()
	void OnRep_Health(float OldHealth);
	UFUNCTION()
	void OnRep_Energy(float OldEnergy);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	UFUNCTION()
	float UpdateHealth(float delta);
	UFUNCTION()
	void UpdateEnergy(float delta);
	UFUNCTION()
	float GetHealth() const;
	UFUNCTION()
	float GetEnergy() const;
};
