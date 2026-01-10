// Fill out your copyright notice in the Description page of Project Settings.


#include "PacMan/PacmanEntity.h"
#include "PaperFlipbookComponent.h"
#include "PacMan/PacmanGame.h"
#include "PacMan/GridMoverComponent.h"

// Sets default values
APacmanEntity::APacmanEntity()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	SpriteComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("Sprite");
	SpriteComponent->SetupAttachment(SceneRoot);
	
	GridMoverComponent = CreateDefaultSubobject<UGridMoverComponent>("GridMoverComponent");
}



void APacmanEntity::BeginPlay()
{
	Super::BeginPlay();
	
	const FVector NewOffset = FVector(0,0,ZOffset);
	SpriteComponent->SetRelativeLocation(NewOffset);
	
}

void APacmanEntity::SetupEntity(APacmanGame* InGameInstance)
{
	if (!InGameInstance) return;

	GameInstance = InGameInstance;
	GridMoverComponent->SetupComponent(this);
	
	if (IsGhost())
	{
		GridMoverComponent->OnExitReachedDelegate.AddDynamic(this, &APacmanEntity::OnExitReached);
		GridMoverComponent->OnSpawnReachedDelegate.AddDynamic(this, &APacmanEntity::OnSpawnReached);
	}
	
}

FIntPoint APacmanEntity::GetSpawnPoint() const
{
	return SpawnGridPoint;
}


void APacmanEntity::RestartEntity()
 {
	GridMoverComponent->Restart();
	
	if (GhostType == EGhostType::Clyde || GhostType == EGhostType::Inky || GhostType == EGhostType::Pinky)
		GhostInstanceState = EGhostInstanceState::InHouse;
	else
		GhostInstanceState = EGhostInstanceState::None;
	
	CurrentSpeedState = ESpeedOfState::Base;
	
	
	
	const FVector NewOffset = FVector(0,0,ZOffset);
	SpriteComponent->SetRelativeLocation(NewOffset);
	
}

void APacmanEntity::ToggleMovement(bool bState)
{
	GridMoverComponent->ToggleMovement(bState);
}

FIntPoint APacmanEntity::GetGridPoint()
{
	return GridMoverComponent->CurrentGridPoint;
}

FIntPoint APacmanEntity::GetHouseEntrance() const
{
	return GameInstance->GetHouseEntrance();
}

EGhostState APacmanEntity::GetGlobalGhostState() const
{
	return GameInstance->GetGhostsState();
}

FIntPoint APacmanEntity::GetBoardDimensions() const
{
	return GameInstance->GetBoardDimensions();
}

APacmanEntity* APacmanEntity::GetPacman() const
{
	return GameInstance->GetPacman();
}

FVector APacmanEntity::GetWorldLocation(FIntPoint Point) const
{
	return GameInstance->GridToWorld(Point);
}

void APacmanEntity::SetFlipbook(UPaperFlipbook* Flipbook)
{
	if (SpriteComponent->GetFlipbook() == Flipbook) return;
	SpriteComponent->SetFlipbook(Flipbook);
	SpriteComponent->PlayFromStart();
}

void APacmanEntity::OnExitReached()
{
	const FVector NewOffset = FVector(0.f,0,ZOffset);
	SpriteComponent->SetRelativeLocation(NewOffset);
	GhostInstanceState = EGhostInstanceState::None;
}

void APacmanEntity::OnSpawnReached()
{
	const FVector NewOffset = FVector(16.f,0,ZOffset);
	SpriteComponent->SetRelativeLocation(NewOffset);
	
	GhostInstanceState = EGhostInstanceState::InHouse;
	SetSpeed(ESpeedOfState::Base);
}
