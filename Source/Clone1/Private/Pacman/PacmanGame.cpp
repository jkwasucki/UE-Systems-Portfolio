// Fill out your copyright notice in the Description page of Project Settings.


#include "PacMan/PacmanGame.h"
#include "PacMan/PacmanPawn.h"
#include "PacMan/GhostPawn.h"
#include "PacMan/PacmanBoardComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"

APacmanGame::APacmanGame()
{
 
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;
	
	PacmanBoard = CreateDefaultSubobject<UPacmanBoardComponent>(TEXT("PacmanBoardComponent"));
	PacmanBoard->SetupAttachment(RootComponent);
	
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2D"));
	SceneCaptureComponent->SetupAttachment(RootComponent);
}

void APacmanGame::BeginPlay()
{
	Super::BeginPlay();

	PacmanBoard->OnCollected.AddDynamic(this, &APacmanGame::IncreaseScore);
}

void APacmanGame::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PacmanBoard)
	{
		PacmanBoard->OnPelletsCollectedDelegate.RemoveAll(this);
	}
	

	Super::EndPlay(EndPlayReason);
}

void APacmanGame::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (!bIsGamePlaying) return;
	
	GlobalModeTimer += DeltaSeconds;
	UpdateGhostState();
	
	
	
	// for (int32 y = 0; y < PacmanBoard->Height; ++y)
	// {
	// 	for (int32 x = 0; x < PacmanBoard->Width; ++x)
	// 	{
	// 		const int32 Index = PacmanBoard->IndexOfTileByCoordinate(x, y);
	// 		if (!PacmanBoard->Tiles.IsValidIndex(Index)) continue;
	//
	// 		const ETileType T = PacmanBoard->Tiles[Index];
	//
	// 		FColor Color = FColor::Red; // default / error
	//
	// 		switch (T)
	// 		{
	// 		case ETileType::Wall:        Color = FColor::Black;   break;
	// 		case ETileType::Empty:       Color = FColor::Silver;  break;
	// 		case ETileType::Pellet:      Color = FColor::Yellow;  break;
	// 		case ETileType::PowerPellet: Color = FColor::Orange;  break;
	// 		case ETileType::Teleport:    Color = FColor::Magenta; break;
	// 		case ETileType::Tunnel:      Color = FColor::Cyan;    break;
	// 		case ETileType::HouseDoor:   Color = FColor::Green;   break;
	// 		}
	//
	// 		const FVector WorldPos =
	// 			PacmanBoard->GridToWorld(FIntPoint(x, y)) + FVector(0, 0, 5.f);
	//
	// 		DrawDebugPoint(
	// 			GetWorld(),
	// 			WorldPos,
	// 			8.f,      // size
	// 			Color,
	// 			false,    // not persistent
	// 			0.f       // one frame
	// 		);
	// 	}
	// }

}

void APacmanGame::UpdateGhostState()
{
	if (CurrentGhostsState == EGhostState::Frightened) return;
	
	EGhostState NewState = EGhostState::None;
	
	if (GlobalModeTimer < 7.f)
		NewState = EGhostState::Scatter;
	else if (GlobalModeTimer < 27.f)
		NewState = EGhostState::Chase;
	else if (GlobalModeTimer < 34.f)
		NewState = EGhostState::Scatter;
	else
		NewState = EGhostState::Chase;

	
	if (CurrentGhostsState != NewState)
	{
		CurrentGhostsState = NewState;
		OnChangeGhostStateDelegate.Broadcast(CurrentGhostsState);
	}
}

void APacmanGame::InitializeGame()
{
	
	if (!PacmanBoard) return;

	UWorld* World = GetWorld();
	if (!World || !World->IsGameWorld()) return;

	APlayerController* PC = World->GetFirstPlayerController();
	AMainPlayerController* MainPC = Cast<AMainPlayerController>(PC);
	
	if (!MainPC) return;
	PlayerController = MainPC;
	
	PacmanBoard->InitializeBoard(this);
	PacmanBoard->OnPelletsCollectedDelegate.RemoveAll(this);
	PacmanBoard->OnPelletsCollectedDelegate.AddDynamic(this, &APacmanGame::RestartGame);
	
	if (!Pacman)
		SpawnPacman();
	if (SpawnedGhosts.Num() == 0)
		SpawnGhosts();
}

void APacmanGame::QuitGame()
{
	StopGame();
	
	bIsGameStopped = false;
	GlobalModeTimer = 0.f;
	CurrentGhostsState = EGhostState::None;
	PreviousGhostsState = EGhostState::None;
	
	PacmanBoard->Reset();	
	Lives = 3;
	Score = 0;
	
	Pacman->RestartEntity();
	
	for (AGhostPawn* Ghost : SpawnedGhosts)
	{
		Ghost->RestartEntity();
	}
	
	OnScoreChangedDelegate.Broadcast(Score);
	OnLivesChangedDelegate.Broadcast(Lives);
	OnGameEndDelegate.Broadcast();
}

void APacmanGame::RestartGame()
{
	bIsGameStopped = false;
	GlobalModeTimer = 0.f;
	CurrentGhostsState = EGhostState::None;
	PreviousGhostsState = EGhostState::None;
	
	if (PacmanBoard->GetPelletCount() == 0 || Lives == 0)
		PacmanBoard->Reset();	
	
	if (Lives == 0)
	{
		Lives = 3;
		Score = 0;
	}
	
	
	
	Pacman->RestartEntity();
	
	for (AGhostPawn* Ghost : SpawnedGhosts)
	{
		Ghost->RestartEntity();
	}
}

void APacmanGame::IncreaseScore(EPacmanCollectibles Collectible)
{
	int32 Value = 0;
	switch (Collectible)
	{
		case EPacmanCollectibles::Pellet: Value = 10;break;
		case EPacmanCollectibles::PowerPellet:
			{
				Value = 50;
				ApplyFear();
			}
	}
	Score += Value;
	OnScoreChangedDelegate.Broadcast(Score);
}

void APacmanGame::ApplyFear()
{
	// Apply and broadcast new state
	CurrentGhostsState = EGhostState::Frightened;
	OnChangeGhostStateDelegate.Broadcast(EGhostState::Frightened);
	
	FTimerDelegate TimerDelegate;
	
	TimerDelegate.BindLambda([this]()
	{
		// Restore previous state
		CurrentGhostsState = PreviousGhostsState;
		OnChangeGhostStateDelegate.Broadcast(CurrentGhostsState);
	});
	
	GetWorld()->GetTimerManager().SetTimer(
		GhostsFearTimerHandle,
		TimerDelegate,
		FearDuration,
		false
	);
} 

bool APacmanGame::IsGamePlaying() { return bIsGamePlaying; }



void APacmanGame::StartGame()
{
	if (bIsGamePlaying) return;
	
	bIsGamePlaying = true;
	
	Pacman->ToggleMovement(true);
	
	TryReleaseGhost();
	
	
	OnGameStartDelegate.Broadcast(); 
	OnScoreChangedDelegate.Broadcast(Score);
	OnLivesChangedDelegate.Broadcast(Lives);
}

void APacmanGame::StopGame()
{
	StopEntities();
	bIsGamePlaying = false;
	bIsGameStopped = true;
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	OnGameStoppedDelegate.Broadcast();
}


void APacmanGame::SpawnPacman()
{
	check(PacmanPawnClass);
	check(PacmanBoard);

	const APacmanPawn* Defaults =
		PacmanPawnClass->GetDefaultObject<APacmanPawn>();

	const FVector SpawnLocation =
		PacmanBoard->GridToWorld(Defaults->SpawnGridPoint);

	const FTransform SpawnTransform(SpawnLocation);
	
	APacmanPawn* PacmanInstance =
		GetWorld()->SpawnActorDeferred<APacmanPawn>(
			PacmanPawnClass,
			SpawnTransform,
			nullptr,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

	check(PacmanInstance);

	PacmanInstance->SetupEntity(this);

	UGameplayStatics::FinishSpawningActor(PacmanInstance, SpawnTransform);

	this->Pacman = PacmanInstance;
}


void APacmanGame::SpawnGhosts()
{
	SpawnedGhosts.Empty();
	for (TSubclassOf<AGhostPawn> GhostClass : GhostClasses)
	{
		if (!GhostClass) continue;
		
		const AGhostPawn* Defaults = GhostClass->GetDefaultObject<AGhostPawn>();
		
		FVector SpawnLocation = PacmanBoard->GridToWorld(Defaults->SpawnGridPoint) + FVector(16.f,0,0);
		
		AGhostPawn* Ghost =
	GetWorld()->SpawnActorDeferred<AGhostPawn>(
		GhostClass,
		FTransform(SpawnLocation),
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

		if (!Ghost) continue;

		Ghost->SetupEntity(this);

		UGameplayStatics::FinishSpawningActor(
			Ghost,
			FTransform(SpawnLocation)
		);

		SpawnedGhosts.Add(Ghost);
	}
}


bool APacmanGame::IsWalkable(FIntPoint XY, APacmanEntity* PacmanEntity)
{
	if (!PacmanEntity) return false;
	const int32 Index = PacmanBoard->IndexOfTileByCoordinate(XY.X, XY.Y);
	if (!PacmanBoard->Tiles.IsValidIndex(Index))
		return false;

	const ETileType TileType = PacmanBoard->Tiles[Index];

	// Pacman rules
	if (PacmanEntity->Entity == EPacmanEntity::Pacman)
	{
		if (TileType == ETileType::Wall || TileType == ETileType::Tunnel || TileType == ETileType::HouseDoor)
			return false;

		if (TileType == ETileType::Pellet || TileType == ETileType::PowerPellet)
			PacmanBoard->RemovePellet(XY);

		return true;
	}

	// Ghost rules
	if (PacmanEntity->Entity == EPacmanEntity::Ghost)
	{
		if (IsHouseDoor(XY))
		{
			if (PacmanEntity->GhostInstanceState == EGhostInstanceState::InHouse ||  PacmanEntity->GhostInstanceState == EGhostInstanceState::Eaten)
				return true;
			else
				return false;
		}
		
		if (TileType == ETileType::Wall) return false;
		
		return true;
	}

	return false;
}

void APacmanGame::TryReleaseGhost()
{
	FTimerDelegate TimerDelegate;
	
	ActivateGhost(EGhostType::Blinky);
	
	TimerDelegate.BindLambda([this]()
	{
		if (!bIsGamePlaying) return;
		
		if (GlobalModeTimer >= 3.f && GlobalModeTimer < 5.f)
			{
				if (!IsGhostActive(EGhostType::Pinky))
				{
					ActivateGhost(EGhostType::Pinky);
				}
			}
		if (Score >= 50)
			{
				if (!IsGhostActive(EGhostType::Inky))
				{
					ActivateGhost(EGhostType::Inky);
				}
			}
		if ( Score >= 100)
			{
				if (!IsGhostActive(EGhostType::Clyde))
				{
					ActivateGhost(EGhostType::Clyde);
				}
			}
		
	});
	
	GetWorld()->GetTimerManager().SetTimer(GhostReleaseTimerHandle, TimerDelegate,1.f,true);
}

AGhostPawn* APacmanGame::GetGhost(EGhostType Type)
{
	for (AGhostPawn* G : SpawnedGhosts)
	{
		if (G->GhostType == Type)
		{
			return G;
		}
	}
	return nullptr;
}

FIntPoint APacmanGame::GetHouseEntrance() const
{
	return HouseEntrancePoint;
}

FIntPoint APacmanGame::GetBoardDimensions() const
{
	return FIntPoint(PacmanBoard->Width, PacmanBoard->Height);
}

APacmanEntity* APacmanGame::GetPacman() const
{
	if (Pacman)
		return Pacman;
	return nullptr;
}

FVector APacmanGame::GridToWorld(FIntPoint Point) const
{
	return PacmanBoard->GridToWorld(Point);
}

float APacmanGame::GetSpeed(ESpeedOfState State) const
{
	switch (State)
	{
		case ESpeedOfState::Frightened: return FearTilesPerSecond * PacmanBoard->TileSize;break;
		case ESpeedOfState::Eaten: return EyesTilesPerSecond * PacmanBoard->TileSize;break;
		case ESpeedOfState::Base: return DefaultTilesPerSecond * PacmanBoard->TileSize;break;
	}
	return DefaultTilesPerSecond * PacmanBoard->TileSize;
}

bool APacmanGame::IsHouseDoor(FIntPoint XY) const
{
	const int32 Index = PacmanBoard->IndexOfTileByCoordinate(XY.X, XY.Y);
	if (!PacmanBoard->Tiles.IsValidIndex(Index))
		return false;

	const ETileType TileType = PacmanBoard->Tiles[Index];


	if (TileType == ETileType::HouseDoor)
		return true;
	else
		return false;
}

bool APacmanGame::GetTeleportExit(const FIntPoint& InPoint, FIntPoint& OutPoint) const
{
	PacmanBoard->GetTeleportExit(InPoint, OutPoint);
	
	if (OutPoint == FIntPoint(27, 16) || OutPoint == FIntPoint(0, 16))
		return true;
	else
		return false;
}

void APacmanGame::ActivateGhost(EGhostType Type)
{
	for (AGhostPawn* G : SpawnedGhosts)
	{
		if (G->GhostType == Type)
		{
			G->Activate();
		}
	}
}

void APacmanGame::StopEntities()
{
	Pacman->ToggleMovement(false);
	for (AGhostPawn* G : SpawnedGhosts)
	{
		G->ToggleMovement(false);
	}
}

void APacmanGame::Play()
{
	if (bIsGameStopped)
	{
		RestartGame();
	}
	
	StartGame();
}

void APacmanGame::CheckForGhostOverlap()
{
	bool HasCollided = false;
	AGhostPawn* Ghost = nullptr;
	for (AGhostPawn* G : SpawnedGhosts)
	{
		if (G->GetGridPoint() == Pacman->GetGridPoint())
		{
			HasCollided = true;
			Ghost = G;
			break;
		}
	}
	if (HasCollided)
	{
		if (Ghost->GhostInstanceState == EGhostInstanceState::Eaten) return;
		
		if (CurrentGhostsState == EGhostState::Frightened)
		{
			Ghost->Eliminate();
		}
		else
		{
			Lives--;
			Pacman->Eliminate();
			OnLivesChangedDelegate.Broadcast(Lives);
			StopGame();
		}
	}
}


bool APacmanGame::IsGhostActive(EGhostType Type)
{
	for (AGhostPawn* G : SpawnedGhosts)
	{
		if (G->GhostType == Type)
		{
			return G->bIsActive;
		}
	}
	return false;
}

EGhostState APacmanGame::GetGhostsState()
{
	return CurrentGhostsState;
}




