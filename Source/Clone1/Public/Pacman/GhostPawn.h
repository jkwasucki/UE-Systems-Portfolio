// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridMoverComponent.h"
#include "PacMan/PacmanEntity.h"
#include "PacMan/PacmanGame.h"
#include "PaperFlipbookComponent.h"

#include "GameFramework/Pawn.h"
#include "PacMan/PacmanTypes.h"
#include "GhostPawn.generated.h"

UCLASS()
class CLONE1_API AGhostPawn : public APacmanEntity
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGhostPawn();

protected:
	virtual void BeginPlay() override;
	
	// VISUALS
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Ghost")
	UPaperFlipbook* SourceFlipbook;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Ghost")
	UPaperFlipbook* FrightenedStateFlipbook;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Ghost")
	UPaperFlipbook* EatenStateFlipbook;

	
	float ReviveDuration = 1.f;
	FTimerHandle ReviveTimerHandle;
	
public:	
	
	UPROPERTY(EditAnywhere)
	bool bIsActive = false;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual bool IsGhost() const override {return true;}
	virtual void OnExitReached() override;
	virtual void OnSpawnReached() override;
	virtual void RestartEntity() override;
	
	virtual void SetupEntity(APacmanGame* GI) override;
	UFUNCTION()
	void SetState(EGhostState State);
	UFUNCTION()
	void Activate();
	UFUNCTION()
	void Eliminate();	
	UFUNCTION()
	void Revive();
	
	// STATE METHODS
	UFUNCTION()
	void FearState();
	UFUNCTION()
	void ScatterChaseState();

	
};

