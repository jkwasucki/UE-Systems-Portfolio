// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSystem/Instances/WeaponInstance.h"
#include "Interfaces/DamageableInterface.h"



void UWeaponInstance::Init(UWeaponDefinition* Definition, APawn* InOwner)
{
	if (!IsValid(Definition)) return;
	if (!IsValid(InOwner)) return;
	
	WeaponDefinition = Definition;
	Owner = InOwner;
	InstanceID = FGuid::NewGuid();
}

void UWeaponInstance::TickWeapon(float DeltaTime)
{
	if (TimeUntilNextShot > 0.f)
		TimeUntilNextShot -= DeltaTime;
	
	
	if (!bIsFiring) return;
	
	if (CurrentFireMode == EWeaponFireMode::SemiAutomatic)
	{
		if (bFiredThisTick)
			return;
	}
	
	if (CanFire())
	{
		TryFiring();
	}
}

void UWeaponInstance::StartFiring()
{
	bIsFiring = true;
	State = EWeaponState::Firing;
	
	OnWeaponRuntimeUpdatedDelegate.Broadcast(this);
}

void UWeaponInstance::StopFiring()
{
	bIsFiring = false;
	bFiredThisTick = false;
	State = EWeaponState::Idle;
	
	OnWeaponRuntimeUpdatedDelegate.Broadcast(this);
}

void UWeaponInstance::TryFiring()
{
	bFiredThisTick = true;
	
	FVector ViewLocation;
	FRotator ViewRotation;
	Owner->GetActorEyesViewPoint(ViewLocation, ViewRotation);
	
	const FVector TraceStart = ViewLocation;
	
	// BASE - NO SPREAD APPLIED
	FVector BaseShotDirection = ViewRotation.Vector();
	
	// APPLY SPREAD
	if (WeaponDefinition->Spread > 0.f)
	{
		const float SpreadRadians = FMath::DegreesToRadians(WeaponDefinition->Spread);
		BaseShotDirection = FMath::VRandCone(BaseShotDirection, SpreadRadians);
	}
	
	const FVector TraceEnd = TraceStart + (BaseShotDirection * WeaponDefinition->Range);
	
	APawn* Pawn = Owner.Get();
	if (!Pawn) return;
	
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Pawn);
	
	
	bool bHit = Owner->GetWorld()->LineTraceSingleByChannel(
		Hit,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		Params
	);
	
	
	DrawDebugHelpers(bHit,Hit, TraceStart, TraceEnd);
	TimeUntilNextShot = WeaponDefinition->TimeBetweenShots;
	
	// Apply damage if hit
	if (bHit)
	{
		
		AActor* HitActor = Hit.GetActor();
		if (!HitActor) return;
		
		
		UE_LOG(LogTemp, Warning,
			TEXT("[Weapon] %s hit %s (Component: %s) for %.1f damage"),
			*GetNameSafe(Owner.Get()),
			*GetNameSafe(HitActor),
			*GetNameSafe(Hit.GetComponent()),
			WeaponDefinition->Damage
		);
		
		if (HitActor->Implements<UDamageableInterface>())
		{
			IDamageableInterface::Execute_TakeDamage(HitActor, WeaponDefinition->Damage);
		}
	}
}
 
bool UWeaponInstance::CanFire()
{
	return TimeUntilNextShot <= 0 ? true : false; 
}



void UWeaponInstance::SetNextFireMode()
{
	if (WeaponDefinition->FireModes.Num() == 0)
		return;

	const TArray<EWeaponFireMode>& Modes = WeaponDefinition->FireModes;

	int32 CurrentIndex = Modes.IndexOfByKey(CurrentFireMode);
	if (CurrentIndex == INDEX_NONE)
	{
		CurrentFireMode = Modes[0];
		return;
	}
	
	int32 NextIndex = (CurrentIndex + 1) % Modes.Num();
	CurrentFireMode = Modes[NextIndex];
	
	OnWeaponRuntimeUpdatedDelegate.Broadcast(this);
}

TWeakObjectPtr<UWeaponDefinition> UWeaponInstance::GetWeaponDefinition()
{
	return WeaponDefinition;
}

FGuid UWeaponInstance::GetInstanceID() const
{
	return InstanceID;
}

EWeaponState UWeaponInstance::GetWeaponState() const
{
	return State;
}

TWeakObjectPtr<APawn> UWeaponInstance::GetOwner() const
{
	return Owner;
}

EWeaponFireMode UWeaponInstance::GetFireMode() const
{
	return CurrentFireMode;
}

void UWeaponInstance::DrawDebugHelpers(bool bIsHit, FHitResult& Hit,  FVector TraceStart,FVector TraceEnd)
{
	APawn* Pawn = Owner.Get();
	if (!Pawn) return;
	
	DrawDebugLine(
		Owner->GetWorld(),
		TraceStart - FVector(0, 0, 5),
		bIsHit ? Hit.ImpactPoint : TraceEnd,
		bIsHit ? FColor::Green : FColor::Red,
		false,
		0.5f,
		0,
		0.5f
	);
	
	DrawDebugSphere(
		Pawn->GetWorld(),
			Hit.ImpactPoint,
			5.0f,
			8,
			FColor::Red,
			false,
			1.0f
	);
}