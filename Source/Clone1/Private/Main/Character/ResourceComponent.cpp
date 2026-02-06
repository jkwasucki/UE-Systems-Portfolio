// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/ResourceComponent.h"

#include "Types/CharacterTypes.h"

// Sets default values for this component's properties
UResourceComponent::UResourceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UResourceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UResourceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
void UResourceComponent::UpdateResource(ECharacterResource Resource, float Delta)
{
	if (Resource == ECharacterResource::None)
		return;
	
	if (Resource ==  ECharacterResource::Health)
		UpdateHealth(Delta);
	if (Resource ==  ECharacterResource::Energy)
		UpdateEnergy(Delta);
}

float UResourceComponent::UpdateHealth(float Delta)
{
	CurrentHealth += Delta;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, 100.f);
	OnHealthChangedDelegate.Broadcast(CurrentHealth);
	SnapshotOnHealthChangedDelegate.Broadcast();
	
	// Broadcast delta
	OnHealthChangedByDeltaDelegate.Broadcast(Delta);
	
	return CurrentHealth;
}

void UResourceComponent::UpdateEnergy(float Delta)
{
	CurrentEnergy += Delta;
	CurrentEnergy = FMath::Clamp(CurrentEnergy, 0.f, 100.f);
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