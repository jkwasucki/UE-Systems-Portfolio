// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Structs/FAbilityAreaOccuranceData.h"
#include "AbilityAreaOccurence.generated.h"

UCLASS()
class CLONE1_API AAbilityAreaOccurence : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	UNiagaraComponent* NiagaraComponent = nullptr;
	UPROPERTY()
	TWeakObjectPtr<AActor> OriginActor = nullptr;
	
	// REP
	UPROPERTY(ReplicatedUsing=OnRep_Initialized)
	bool bInitialized = false;
	UPROPERTY(Replicated)
	TWeakObjectPtr<AActor> RepOriginActor;
	UPROPERTY(Replicated)
	FAbilityAreaOccurenceData RepAreaOccurenceData;
	UPROPERTY(Replicated)
	FVector RepSpawnLocation;
	
	
public:	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UNiagaraSystem* NiagaraSystem;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	USphereComponent* Collider;
	
	UPROPERTY(VisibleAnywhere)
	FAbilityAreaOccurenceData AreaOccurenceData;
	
protected:
	UFUNCTION()
	void OnRep_Initialized();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
public:
	AAbilityAreaOccurence();
	
	UFUNCTION()
	void OnOverlapped(  UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	UFUNCTION()
	void SpawnNiagara(FVector SpawnLocation);
	UFUNCTION()
	void InitializeAreaOccurence(AActor* InOriginActor,FAbilityAreaOccurenceData& Data, FVector& SpawnLocation);
	
};
