// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pacman/PacmanTypes.h"
#include "GridMoverComponent.h"
#include "GameFramework/Pawn.h"
#include "PacmanEntity.generated.h"
class APacmanGame;
class UPaperFlipbookComponent; 
class UPaperFlipbook;
class APacmanPawn;
UCLASS()
class CLONE1_API APacmanEntity : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APacmanEntity();

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneRoot;
	UPROPERTY(VisibleAnywhere)
	UPaperFlipbookComponent* SpriteComponent;
	
	
	virtual void BeginPlay() override;
public:	
	
	ESpeedOfState CurrentSpeedState = ESpeedOfState::Base;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EGhostInstanceState GhostInstanceState;
	
	// SETTINGS
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ZOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntPoint SpawnGridPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGhostType GhostType;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	EPacmanEntity Entity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UGridMoverComponent* GridMoverComponent;
	UPROPERTY()
	APacmanGame* GameInstance = nullptr;
	
	
	virtual void RestartEntity();
	
	
	UFUNCTION()
	void ToggleMovement(bool bState);
	UFUNCTION()
	virtual void SetupEntity(APacmanGame* GI);
	UFUNCTION()
	void SetFlipbook(UPaperFlipbook* Flipbook);
	UFUNCTION()
	void SetSpeed(ESpeedOfState State) { CurrentSpeedState = State; }
	UFUNCTION()
	virtual void OnExitReached();
	UFUNCTION()
	virtual void OnSpawnReached();
	
	UFUNCTION()
	virtual bool IsPacman() const {return false;}
	UFUNCTION()
	virtual bool IsGhost() const {return false;}
	
	// GETTERS
	UFUNCTION()
	FIntPoint GetSpawnPoint() const;
	UFUNCTION()
	FIntPoint GetGridPoint();
	UFUNCTION()
	FIntPoint GetHouseEntrance() const;
	UFUNCTION()
	EGhostState GetGlobalGhostState() const;
	UFUNCTION()
	FIntPoint GetBoardDimensions() const;
	UFUNCTION()
	APacmanEntity* GetPacman() const;
	UFUNCTION()
	FVector GetWorldLocation(FIntPoint Point) const;
};
