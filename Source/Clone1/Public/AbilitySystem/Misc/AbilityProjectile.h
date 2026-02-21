// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ProjectileMoverComponent.h"
#include "GameFramework/Actor.h"
#include "Structs/FAbilityProjectileData.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"

#include "AbilityProjectile.generated.h"

UCLASS()
class CLONE1_API AAbilityProjectile : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UProjectileMoverComponent* ProjectileMoverComponent = nullptr;
	UPROPERTY()
	TWeakObjectPtr<AActor> OriginActor = nullptr;
	
	
	// Components
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UNiagaraSystem* NiagaraSystem;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	USphereComponent* Collider;
	UPROPERTY()
	UNiagaraComponent* NiagaraComponent = nullptr;
	
	
	UPROPERTY(VisibleAnywhere)
	FAbilityProjectileData ProjectileData;
	
	
	// REP
	UPROPERTY(ReplicatedUsing=OnRep_Initialized)
	bool bIsInitialized = false;
	UPROPERTY(Replicated)
	TWeakObjectPtr<AActor> RepOriginActor;
	UPROPERTY(Replicated)
	FAbilityProjectileData RepProjectileData;
	UPROPERTY(Replicated)
	FAbilityTargetData RepTargetData;

protected:
	UFUNCTION()
	void SpawnHitNiagara();
	UFUNCTION()
	void SpawnNiagara();
	UFUNCTION()
	void OnRep_Initialized();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	AAbilityProjectile();
	UFUNCTION()
	void InitializeProjectile(AActor* ProjectileOrigin,FAbilityProjectileData& Data,FAbilityTargetData& TargetData);
	UFUNCTION()
	void OnOverlapped(  UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};


