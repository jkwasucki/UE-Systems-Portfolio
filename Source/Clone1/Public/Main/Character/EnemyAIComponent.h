// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Main/Character/Base/BaseCharacter.h"
#include "EnemyAIComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UEnemyAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyAIComponent();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY()
	ABaseCharacter* BaseCharacter = nullptr;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	USphereComponent* AIAttackTriggerZone;
	
	UPROPERTY()
	TArray<AActor*> FoesInRange;
	
	UFUNCTION()
	void HandleState();
	UFUNCTION()
	bool IsFoeInRange();
	UFUNCTION()
	void TryAttacking();
	UFUNCTION()
	void FaceFoe(float DeltaTime);
	
public:	
	
	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	
	UFUNCTION()
	void Initialize(ABaseCharacter* InBaseCharacter);
	
	UFUNCTION()
	void OnFoeInRange( UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);
	UFUNCTION()
	void OnFoeOutOfRange(   UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex);
	
	
	
	UFUNCTION()
	AActor* GetClosestFoe();
	
};
