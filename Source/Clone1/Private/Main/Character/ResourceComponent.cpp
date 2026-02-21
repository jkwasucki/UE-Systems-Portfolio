// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/ResourceComponent.h"
#include "Net/UnrealNetwork.h"


UResourceComponent::UResourceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UResourceComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UResourceComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UResourceComponent, CurrentHealth);
	DOREPLIFETIME(UResourceComponent, CurrentEnergy);
}

float UResourceComponent::UpdateHealth(float delta)
{
	CurrentHealth += delta;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, 100.f);
	
	OnHealthChangedDelegate.Broadcast(CurrentHealth);
	SnapshotOnHealthChangedDelegate.Broadcast();
	
	// Broadcast delta
	OnHealthChangedByDeltaDelegate.Broadcast(delta);
	
	return CurrentHealth;
}
void UResourceComponent::UpdateEnergy(float Delta)
{
	CurrentEnergy += Delta;
	CurrentEnergy = FMath::Clamp(CurrentEnergy, 0.f, 100.f);
	OnEnergyChangedDelegate.Broadcast(CurrentEnergy);
	SnapshotOnEnergyChangedDelegate.Broadcast();
}

void UResourceComponent::OnRep_Health(float OldHealth)
{
	float Delta = CurrentHealth - OldHealth;
	
	OnHealthChangedDelegate.Broadcast(CurrentHealth);
	SnapshotOnHealthChangedDelegate.Broadcast();

	OnHealthChangedByDeltaDelegate.Broadcast(Delta);
}

void UResourceComponent::OnRep_Energy(float OldEnergy)
{
	OnEnergyChangedDelegate.Broadcast(CurrentEnergy);
	SnapshotOnEnergyChangedDelegate.Broadcast();
}


float UResourceComponent::GetHealth() const
{
	return CurrentHealth;
}

float UResourceComponent::GetEnergy() const
{
	return CurrentEnergy;
}