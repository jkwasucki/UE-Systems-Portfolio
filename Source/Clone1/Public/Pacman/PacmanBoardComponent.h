// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PacMan/PacmanEntity.h"
#include "Pacman/PacmanTypes.h"
#include "PaperGroupedSpriteComponent.h"
#include "Components/SceneComponent.h"
#include "PacmanBoardComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPelletsCollected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCollected, EPacmanCollectibles, Collectible);
class APacmanGame;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CLONE1_API UPacmanBoardComponent : public USceneComponent
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	UPacmanBoardComponent();

protected:
	UFUNCTION()
	void InitFromAscii();
	
	UPROPERTY()
	int32 PelletCount; 
	
public:	
	UPROPERTY()
	APacmanGame* GameInstance = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPaperGroupedSpriteComponent* PelletSprites;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Pellet")
	UPaperSprite* PelletSprite;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Pellet")
	UPaperSprite* PowerPelletSprite;
	
	
	
	static const TCHAR* Maze[31];
	
	UPROPERTY(EditAnywhere)
	TArray<ETileType> Tiles;
	
	UPROPERTY()
	int32 Width = 28;
	UPROPERTY()
	int32 Height = 31;
	
	UPROPERTY()
	float TileSize = 32.0f;
	
	UFUNCTION()
	int32 GetPelletCount() const;
	UFUNCTION()
	void InitializeBoard(APacmanGame* GI);
	UFUNCTION()
	void SpawnPellets();
	UFUNCTION()
	void RemovePellet(FIntPoint GridPos);
	UFUNCTION()
	bool GetTeleportExit(const FIntPoint& InPoint, FIntPoint& OutPoint) const;
	UFUNCTION()
	void TeleportGhost(APacmanEntity* Entity, FIntPoint& InPoint);
	UFUNCTION()
	FVector GridToWorld(const FIntPoint& GridPos);
	UFUNCTION()
	int32 IndexOfTileByCoordinate(int32 X, int32 Y) const;
	UFUNCTION()
	void Reset();
	
	FOnPelletsCollected OnPelletsCollectedDelegate;
	FOnCollected OnCollected;

};

inline int32 UPacmanBoardComponent::IndexOfTileByCoordinate(int32 X, int32 Y) const
{
	return Y * Width + X;
}
