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
	
public:	
	// Sets default values for this actor's properties
	AAbilityProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
	UProjectileMoverComponent* ProjectileMoverComponent = nullptr;
	UPROPERTY()
	AActor* OriginActor = nullptr;
public:	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UNiagaraSystem* NiagaraSystem;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	USphereComponent* Collider;
	UPROPERTY()
	UNiagaraComponent* NiagaraComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	FAbilityProjectileData ProjectileData;
	
	UFUNCTION()
	void SpawnNiagara();
	UFUNCTION()
	void InitializeProjectile(AActor* ProjectileOrigin,FAbilityProjectileData& Data,FAbilityTargetData& TargetData);
	UFUNCTION()
	void OnOverlapped(  UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);
	UFUNCTION()
	void SpawnHitNiagara();
};


