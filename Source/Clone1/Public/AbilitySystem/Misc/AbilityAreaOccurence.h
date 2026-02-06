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
	AActor* OriginActor = nullptr;
public:	
	// Sets default values for this actor's properties
	AAbilityAreaOccurence();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UNiagaraSystem* NiagaraSystem;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	USphereComponent* Collider;
	
	
	UPROPERTY(VisibleAnywhere)
	FAbilityAreaOccurenceData AreaOccurenceData;
	
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
