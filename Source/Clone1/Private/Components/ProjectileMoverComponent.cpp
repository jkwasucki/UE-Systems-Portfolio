// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ProjectileMoverComponent.h"

#include "Structs/FAbilityTargetData.h"

// Sets default values for this component's properties
UProjectileMoverComponent::UProjectileMoverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UProjectileMoverComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UProjectileMoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	
	if (!bCanMove) return;
	
	
	if (MoveDirection.IsNearlyZero())
		return;

	
	const FVector DeltaMove = MoveDirection * Speed * DeltaTime;

	if (AActor* Owner = GetOwner())
	{
		Owner->AddActorWorldOffset(DeltaMove, true);
		TraveledDistance += DeltaMove.Size();

		if (TraveledDistance >= MaxRange)
		{
			Owner->Destroy();
		}
	}
}

void UProjectileMoverComponent::InitializeFromTargetData(const FAbilityTargetData& TargetData)
{
	if (!TargetData.bHasValidTarget)
	{
		UE_LOG(LogTemp, Error, TEXT("Projectile initialized with invalid TargetData"));
		return;
	}

	MoveDirection = TargetData.Direction.GetSafeNormal();

	if (AActor* Owner = GetOwner())
	{
		Owner->SetActorRotation(MoveDirection.Rotation());
	}
	bCanMove = true;
}

void UProjectileMoverComponent::Stop()
{
	bCanMove = false;
}

