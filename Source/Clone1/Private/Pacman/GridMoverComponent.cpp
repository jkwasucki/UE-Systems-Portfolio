// Fill out your copyright notice in the Description page of Project Settings.


#include "PacMan/GridMoverComponent.h"
#include "Types/GenericTypes.h"
#include "PacMan/PacmanGame.h"
#include "PacMan/PacmanEntity.h"



UGridMoverComponent::UGridMoverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGridMoverComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(true);


}

const TArray<EMoveDirection>& UGridMoverComponent::GetAllDirs()
{
	static const TArray<EMoveDirection> AllDirs =
	{
		EMoveDirection::UP,
		EMoveDirection::DOWN,
		EMoveDirection::LEFT,
		EMoveDirection::RIGHT
	};
	
	return AllDirs;
}


void UGridMoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!Parent || !Parent->GameInstance->IsGamePlaying()) return;
	
	
	if (!bCanMove) return;
	
	
	if (Parent->Entity == EPacmanEntity::Ghost)
	{
		if (!bIsMoving)
		{
			// If we just became EATEN, do the forced reversal first
			// Do NOT overwrite DesiredDirection with greedy yet
			if (Parent->GhostInstanceState == EGhostInstanceState::Eaten &&
				bAllowEatenReverseOnce)
			{
				// DesiredDirection was already set in OnBecameEaten()
				// so we do nothing here
			}
			else if (Parent->GhostInstanceState == EGhostInstanceState::InHouse)
			{
				SetDirection(ChooseGreedyDir(Parent->GetHouseEntrance()));
			}
			else if (Parent->GetGlobalGhostState() == EGhostState::Frightened)
			{
				ChooseRandomDir();
			}
			else
			{
				FIntPoint Target = GetTargetTile();
				SetDirection(ChooseGreedyDir(Target));
			}
		}
	}

	if (!bIsMoving)
	{
		TryStartMove();
		return;
	}

	const float Distance = FVector::Distance(StartLocation, TargetLocation);
	const float Speed = Parent->GameInstance->GetSpeed(Parent->CurrentSpeedState);

	MoveProgress += (Speed * DeltaTime) / Distance;

	const float Alpha = FMath::Clamp(MoveProgress, 0.f, 1.f);
	GetOwner()->SetActorLocation(FMath::Lerp(StartLocation, TargetLocation, Alpha));

	if (Alpha >= 1.f)
	{
		bIsMoving = false;
		HandleSpawnReached();
		HandleExitReached();
	}
}

void UGridMoverComponent::ToggleMovement(bool bstate)
{
	bCanMove =  bstate;
}

void UGridMoverComponent::Restart()
{
	CurrentGridPoint = Parent->SpawnGridPoint;
	CurrentDirection = EMoveDirection::LEFT;
	DesiredDirection = EMoveDirection::LEFT;
	bAllowEatenReverseOnce = false;
	bIsMoving = false;
	
	FVector Offset = Parent->Entity == EPacmanEntity::Ghost ? FVector(16.f,0,0) : FVector::ZeroVector;
	FVector SpawnPos = Parent->GetWorldLocation(Parent->SpawnGridPoint) + Offset;
	Parent->SetActorLocation(SpawnPos);
}

void UGridMoverComponent::SetupComponent(APacmanEntity* PE)
{
	if (!PE) return;
	Parent = PE;
	CurrentGridPoint = Parent->SpawnGridPoint;
	CurrentDirection = EMoveDirection::LEFT;
}
void UGridMoverComponent::OnBecameEaten()
{
	bAllowEatenReverseOnce = true;
	DesiredDirection = Opposite(CurrentDirection);
}

void UGridMoverComponent::TryStartMove()
{
	Parent->GameInstance->CheckForGhostOverlap();
	
	if (Parent->GameInstance->IsWalkable(AquireTargetLocation(DesiredDirection), Parent))
	{
		// If we are EATEN and we are taking the opposite direction, consume the one-time reversal.
		if (Parent->GhostInstanceState == EGhostInstanceState::Eaten &&
			DesiredDirection == Opposite(CurrentDirection) &&
			bAllowEatenReverseOnce)
		{
			bAllowEatenReverseOnce = false;
		}

		CurrentDirection = DesiredDirection;
	}

	FIntPoint NextGrid = AquireTargetLocation(CurrentDirection);
	

	if (!Parent->GameInstance->IsWalkable(NextGrid,Parent))
		return;
	
	
	// TELEPORT
	FIntPoint TeleportExit;
	if (Parent->GameInstance
		->GetTeleportExit(NextGrid, TeleportExit))
	{
		NextGrid = TeleportExit;
		
		const FVector ExitWorld =
			Parent->GetWorldLocation(TeleportExit);

		Parent->SetActorLocation(ExitWorld);
	}
	
	
	OnDirectionValidatedDelegate.Broadcast(CurrentDirection);
	
	StartLocation = Parent->GetActorLocation();
	TargetLocation = Parent->GetWorldLocation(NextGrid);
	CurrentGridPoint = NextGrid;
	
	MoveProgress = 0.f;
	bIsMoving = true;
}





void UGridMoverComponent::SetDirection(EMoveDirection Dir)
{
	if (!bCanMove) return;
	DesiredDirection = Dir;
}

FIntPoint UGridMoverComponent::VectorFromDirection(EMoveDirection Dir)
{
	int32 XDir = 0;
	int32 YDir = 0;
	
	switch (Dir)
	{
	case EMoveDirection::LEFT:
		XDir = 1;
		break;

	case EMoveDirection::RIGHT:
		XDir = -1;
		break;

	case EMoveDirection::UP:
		YDir = 1;
		break;

	case EMoveDirection::DOWN:
		YDir = -1;
		break;

	default:
		break;
	}
	return FIntPoint(XDir, YDir);
}

FIntPoint UGridMoverComponent::AquireTargetLocation(EMoveDirection Dir)
{
	int32 CurrentLocX = CurrentGridPoint.X;
	int32 CurrentLocY = CurrentGridPoint.Y;
	
	FIntPoint Vector = VectorFromDirection(Dir);
	
	
	
	return FIntPoint(
	FMath::Clamp(CurrentLocX + Vector.X, 0, Parent->GetBoardDimensions().X - 1),
	FMath::Clamp(CurrentLocY + Vector.Y, 0, Parent->GetBoardDimensions().Y - 1)
	);
}

TArray<EMoveDirection> UGridMoverComponent::AquireValidDirections()
{
	TArray<EMoveDirection> ValidDirections;

	for (EMoveDirection Dir : GetAllDirs())
	{
		const bool bIsOpposite = (Dir == Opposite(CurrentDirection));

		// Default Pac-Man rule: no reverse.
		// Exception: EATEN ghosts may reverse only once (right after becoming EATEN).
		if (bIsOpposite)
		{
			const bool bCanReverseNow =
				(Parent->GhostInstanceState == EGhostInstanceState::Eaten) &&
				bAllowEatenReverseOnce;

			if (!bCanReverseNow)
			{
				continue;
			}
		}

		FIntPoint Next = AquireTargetLocation(Dir);
		if (Parent->GameInstance->IsWalkable(Next, Parent))
		{
			ValidDirections.Add(Dir);
		}
	}

	return ValidDirections;
}


EMoveDirection UGridMoverComponent::Opposite(EMoveDirection Dir)
{
	switch (Dir)
	{
	case EMoveDirection::LEFT: return EMoveDirection::RIGHT; break;
	case EMoveDirection::RIGHT: return EMoveDirection::LEFT; break;
	case EMoveDirection::UP: return EMoveDirection::DOWN; break;
	case EMoveDirection::DOWN: return EMoveDirection::UP; break;
	default: return EMoveDirection::LEFT; break;
	}
}


FIntPoint UGridMoverComponent::GetTargetTile() const
{
	//EATEN → ghost house
	if (Parent->GhostInstanceState == EGhostInstanceState::Eaten)
	{
		return Parent->GetSpawnPoint();
	}

	//SCATTER → corner tile
	if (Parent->GetGlobalGhostState() == EGhostState::Scatter)
	{
		return GetScatterCorner();
	}

	//FRIGHTENED → meaningless target (movement is random)
	if (Parent->GetGlobalGhostState() == EGhostState::Frightened)
	{
		return CurrentGridPoint;
	}

	//CHASE → depends on ghost type
	return GetChaseTarget();
}


FIntPoint UGridMoverComponent::GetScatterCorner() const
{
	const int32 W = Parent->GetBoardDimensions().X - 1;
	const int32 H = Parent->GetBoardDimensions().Y - 1;

	switch (Parent->GhostType)
	{
	case EGhostType::Blinky: return FIntPoint(W, H);      // top-right
	case EGhostType::Pinky:  return FIntPoint(0, H);      // top-left
	case EGhostType::Inky:   return FIntPoint(W, 0);      // bottom-right
	case EGhostType::Clyde:  return FIntPoint(0, 0);      // bottom-left
	default:                 return FIntPoint(W, H);
	}
}
FIntPoint UGridMoverComponent::GetChaseTarget() const
{
	APacmanEntity* Pacman = Parent->GetPacman();

	if (!Pacman)
		return CurrentGridPoint;

	switch (Parent->GhostType)
	{
	case EGhostType::Blinky:
		{
		
			return Pacman->GetGridPoint();
		}

		// others later
	default:
		return Pacman->GetGridPoint();
	}
}

void UGridMoverComponent::HandleSpawnReached()
{
	if (Parent->GhostInstanceState != EGhostInstanceState::Eaten) return;
	
	if (CurrentGridPoint == Parent->GetSpawnPoint() && bCanMove)
	{
		ToggleMovement(false);
		OnSpawnReachedDelegate.Broadcast();
		return;
	}
}

void UGridMoverComponent::HandleExitReached()
{
	if (Parent->GhostInstanceState != EGhostInstanceState::InHouse) return;
	if (CurrentGridPoint == Parent->GetHouseEntrance())
	{
		OnExitReachedDelegate.Broadcast();
		return;
	}
}

EMoveDirection UGridMoverComponent::ChooseGreedyDir(const FIntPoint& Target)
{
	TArray<EMoveDirection> ValidDirs = AquireValidDirections();
	if (ValidDirs.Num() == 0)
	{
		return Opposite(CurrentDirection);
	}

	float BestDist = TNumericLimits<float>::Max();
	EMoveDirection BestDir = CurrentDirection;

	for (EMoveDirection Dir : ValidDirs)
	{
		FIntPoint NextTile = AquireTargetLocation(Dir);

		// Manhattan distance works better for grids
		float Dist =
			FMath::Abs(NextTile.X - Target.X) +
			FMath::Abs(NextTile.Y - Target.Y);

		// Pick strictly better distance
		if (Dist < BestDist)
		{
			BestDist = Dist;
			BestDir = Dir;
		}
		// Tie-breaker: prefer continuing current direction
		else if (FMath::IsNearlyEqual(Dist, BestDist) &&
				 Dir == CurrentDirection)
		{
			BestDir = Dir;
		}
	}

	return BestDir;
}



void UGridMoverComponent::ChooseRandomDir()
{
	TArray<EMoveDirection> ValidDirections = AquireValidDirections();
	if (ValidDirections.Num() <= 0) return;
	
	const int32 Index = FMath::RandRange(0, ValidDirections.Num() - 1);
	SetDirection(ValidDirections[Index]);
}


