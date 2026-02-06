// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/CharacterMoverComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Main/Character/Derived/MainCharacter.h"

// Sets default values for this component's properties
UCharacterMoverComponent::UCharacterMoverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCharacterMoverComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!IsCharacterMoving())
	{
		if (bIsMoving)
		{
			bIsMoving = false;
			OnStateRequestDelegate.ExecuteIfBound(EEntityState::Idle);
		}
	}
	else
	{
		if (!bIsMoving)
		{
			bIsMoving = true;
			OnStateRequestDelegate.ExecuteIfBound(EEntityState::Moving);
		}
	}
}

void UCharacterMoverComponent::ToggleMovement(bool bState)
{
	ACharacter* MainCharacter = Cast<ACharacter>(GetOwner());
	if (!MainCharacter)
		return;

	UCharacterMovementComponent* MoveComp = MainCharacter->GetCharacterMovement();
	if (!MoveComp)
		return;

	if (bState)
	{
		MoveComp->SetMovementMode(MOVE_Walking);
	}
	else
	{
		MoveComp->DisableMovement();
	}
}


void UCharacterMoverComponent::UpdateSpeed()
{
	ACharacter* MainCharacter = Cast<ACharacter>(GetOwner());
	if (!MainCharacter)
		return;
	
	float Speed = WeightSpeedMultiplier * AttributeSpeedMultiplier;
	
	MainCharacter->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed * Speed;
}

void UCharacterMoverComponent::ModifySpeedByWeight(int32 CurrentWeight, int32 MaxWeight)
{
	ACharacter* MainCharacter = Cast<ACharacter>(GetOwner());
	if (!MainCharacter)
		return;
	
	float NewSpeed = BaseWalkSpeed;
	if (CurrentWeight > MaxWeight)
	{
		const float OverRation = float(CurrentWeight - MaxWeight) / float(MaxWeight);
		WeightSpeedMultiplier = FMath::Clamp(1.f - OverRation * WeightStrengthMultiplier, 0.3f, 1.f);
	}
	else
	{
		WeightSpeedMultiplier = 1.f;
	}

	UpdateSpeed();
}

bool UCharacterMoverComponent::IsCharacterMoving()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character && Character->GetCharacterMovement())
	{
		return Character->GetCharacterMovement()->Velocity.SizeSquared() > 0.1f;
	}
	return false;
}
