// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/GenericTypes.h"
#include "Components/ActorComponent.h"
#include "GridMoverComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDirectionValidated, EMoveDirection, Dir);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnReached);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitReached);

class APacmanEntity;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent = false) )
class CLONE1_API UGridMoverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridMoverComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	static const TArray<EMoveDirection>& GetAllDirs();
	
	
	UPROPERTY()
	bool bAllowEatenReverseOnce = false;
public:	
	
	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY()
	APacmanEntity* Parent = nullptr;
	
	UPROPERTY()
	bool bCanMove = false;
	
	bool bIsMoving = false;
	
	UPROPERTY()
	EMoveDirection CurrentDirection = EMoveDirection::LEFT;
	UPROPERTY()
	EMoveDirection DesiredDirection;
	
	float MoveProgress = 0.f;
	FVector StartLocation;
	FVector TargetLocation;
	
	UPROPERTY()
	FIntPoint CurrentGridPoint = FIntPoint(1,1);
	
	
	UFUNCTION()
	void ToggleMovement(bool bstate);
	UFUNCTION()
	void Restart();
	UFUNCTION()
	void SetupComponent(APacmanEntity* PE);

	UFUNCTION()
	void TryStartMove();
	UFUNCTION()
	void OnBecameEaten();
	UFUNCTION()
	void SetDirection(EMoveDirection Dir);
	UFUNCTION()
	FIntPoint VectorFromDirection(EMoveDirection Dir);
	UFUNCTION()
	FIntPoint AquireTargetLocation(EMoveDirection Dir);
	UFUNCTION()
	TArray<EMoveDirection> AquireValidDirections() ;
	UFUNCTION()
	EMoveDirection Opposite(EMoveDirection Dir);
	UFUNCTION()
	void ChooseRandomDir();
	UFUNCTION()
	EMoveDirection ChooseGreedyDir(const FIntPoint& Target);
	UFUNCTION()
	FIntPoint GetTargetTile() const;
	UFUNCTION()
	FIntPoint GetScatterCorner() const;
	UFUNCTION()
	FIntPoint GetChaseTarget() const;
	UFUNCTION()
	void HandleSpawnReached();
	UFUNCTION()
	void HandleExitReached();
	
	
	FOnDirectionValidated OnDirectionValidatedDelegate;
	FOnSpawnReached OnSpawnReachedDelegate;
	FOnExitReached OnExitReachedDelegate;
};
