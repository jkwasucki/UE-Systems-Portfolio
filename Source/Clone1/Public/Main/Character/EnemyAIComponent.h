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


protected:
	UPROPERTY()
	TWeakObjectPtr<ABaseCharacter> BaseCharacter;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	USphereComponent* AIAttackTriggerZone;
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> FoesInRange;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	UFUNCTION()
	void HandleState();
	UFUNCTION()
	bool IsFoeInRange();
	UFUNCTION()
	void TryAttacking();
	UFUNCTION()
	void FaceFoe(float DeltaTime);
public:
	UEnemyAIComponent();
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
