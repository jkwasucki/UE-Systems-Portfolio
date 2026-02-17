// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/WeaponTypes.h"
#include "WeaponSystem/Definitions/WeaponDefinition.h"
#include "WeaponInstance.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponRuntimeUpdated, UWeaponInstance*, This);

UCLASS()
class CLONE1_API UWeaponInstance : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TWeakObjectPtr<APawn> Owner = nullptr;
	UPROPERTY()
	TWeakObjectPtr<UWeaponDefinition> WeaponDefinition = nullptr;
	UPROPERTY()
	FGuid InstanceID;
	UPROPERTY()
	EWeaponState State = EWeaponState::Idle;
	
	UPROPERTY()
	bool bIsFiring = false;
	UPROPERTY()
	float TimeUntilNextShot = 0.f;
	UPROPERTY()
	bool bFiredThisTick = false;
	
	UPROPERTY()
	EWeaponFireMode CurrentFireMode = EWeaponFireMode::Automatic;

private:
	void TryFiring();
	bool CanFire();
	void DrawDebugHelpers(bool bIsHit, FHitResult& Hit,  FVector TraceStart,FVector TraceEnd);
public:
	
	UPROPERTY()
	FOnWeaponRuntimeUpdated OnWeaponRuntimeUpdatedDelegate;
	
	UFUNCTION()
	void Init(UWeaponDefinition* Definition, APawn* InOwner);
	UFUNCTION()
	void StartFiring();
	UFUNCTION()
	void StopFiring();
	UFUNCTION()
	void TickWeapon(float deltaTime);
	UFUNCTION()
	void SetNextFireMode();
	
	UFUNCTION()
	TWeakObjectPtr<UWeaponDefinition> GetWeaponDefinition();
	UFUNCTION()
	FGuid GetInstanceID() const;
	UFUNCTION()
	EWeaponState GetWeaponState() const;
	UFUNCTION()
	TWeakObjectPtr<APawn> GetOwner() const;
	UFUNCTION()
	EWeaponFireMode GetFireMode() const;
	
};