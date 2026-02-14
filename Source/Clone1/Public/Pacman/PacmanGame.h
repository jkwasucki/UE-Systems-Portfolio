

#pragma once

#include "CoreMinimal.h"
#include "Main/PlayerController/MainPlayerController.h"
#include "PacmanBoardComponent.h"
#include "GameFramework/Actor.h"
#include "PacmanGame.generated.h"
class APacmanPawn;
class AGhostPawn;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeGhostState, EGhostState, GlobalState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStopped);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLivesChanged, int32, CurrentLives);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, CurrentScore);

// TODO: Other chasing strategies for ghosts, Granting score for eating ghosts 
// (BUG) When Player wins, game restarts but player cant move


UCLASS()
class CLONE1_API APacmanGame : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	USceneComponent* SceneRoot = nullptr;
	UPROPERTY()
	APacmanPawn* Pacman = nullptr;
	UPROPERTY()
	TArray<AGhostPawn*> SpawnedGhosts;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APacmanPawn> PacmanPawnClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pacman|Ghosts")
	TArray<TSubclassOf<AGhostPawn>> GhostClasses;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPacmanBoardComponent* PacmanBoardComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	UTextureRenderTarget2D* RenderTarget;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Camera")
	USceneCaptureComponent2D* SceneCaptureComponent;
	
	
	
	FIntPoint HouseEntrancePoint = FIntPoint(13,19); // Just before doors
	
	// GHOSTS GLOBAL STATE
	EGhostState CurrentGhostsState = EGhostState::None;
	EGhostState PreviousGhostsState = EGhostState::None;
	
	const float FearDuration = 6.0f;
	
	// TILES/s
	float DefaultTilesPerSecond = 10.f;
	float EyesTilesPerSecond = 25.f;
	float FearTilesPerSecond = 5.0f;
	
	UPROPERTY()
	bool bIsGameStopped = false;
	UPROPERTY()
	bool bIsGamePlaying = false;
	UPROPERTY()
	int32 Score = 0;
	UPROPERTY()
	float GlobalModeTimer = 0.0f;
	UPROPERTY()
	int32 Lives = 3;
	
	FTimerHandle GhostReleaseTimerHandle;
	FTimerHandle GhostsFearTimerHandle;

public:
	UPROPERTY()
	AMainPlayerController* PlayerController = nullptr;
	
	//Delegates
	FOnGameStart OnGameStartDelegate;
	FOnGameEnd OnGameEndDelegate;
	FOnChangeGhostState OnChangeGhostStateDelegate;
	FOnLivesChanged OnLivesChangedDelegate;
	FOnScoreChanged OnScoreChangedDelegate;
	FOnGameStopped OnGameStoppedDelegate;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick( float DeltaSeconds ) override;
	UFUNCTION()
	void UpdateGhostState();
	UFUNCTION()
	void SpawnPacman();
	UFUNCTION()
	void SpawnGhosts();
	UFUNCTION()
	void StartGame();
	UFUNCTION()
	void StopGame();
	UFUNCTION()
	void RestartGame();
	UFUNCTION()
	void IncreaseScore(EPacmanCollectibles Collectibl);
	UFUNCTION()
	void ApplyFear();
	UFUNCTION()
	void TryReleaseGhost();
	UFUNCTION()
	void ActivateGhost(EGhostType Type);
	UFUNCTION()
	void StopEntities();

public:
	APacmanGame();
	UFUNCTION()
	void InitializeGame();
	UFUNCTION()
	void QuitGame();
	UFUNCTION()
	void Play();
	
	// API
	UFUNCTION()
	void CheckForGhostOverlap();
	UFUNCTION()
	bool IsGhostActive(EGhostType Type);
	UFUNCTION()
	EGhostState GetGhostsState();
	UFUNCTION()
	bool IsGamePlaying();
	UFUNCTION()
	bool IsWalkable(FIntPoint XY,APacmanEntity* PacmanEntity);
	UFUNCTION()
	inline int32 GetScore() const {return Score;}
	UFUNCTION()
	AGhostPawn* GetGhost(EGhostType Type);
	UFUNCTION()
	FIntPoint GetHouseEntrance() const;
	UFUNCTION()
	FIntPoint GetBoardDimensions() const;
	UFUNCTION()
	APacmanEntity* GetPacman() const;
	UFUNCTION()
	FVector GridToWorld(FIntPoint Point) const;
	UFUNCTION()
	float GetSpeed(ESpeedOfState State) const;
	UFUNCTION()
	bool IsHouseDoor(FIntPoint Point) const;
	UFUNCTION()
	bool GetTeleportExit(const FIntPoint& InPoint, FIntPoint& OutPoint) const;
};
