#pragma once
#include "CoreMinimal.h"
#include "PacmanTypes.generated.h"
UENUM(BlueprintType)
enum class EPacmanEntity : uint8
{
	None,
	Pacman,
	Ghost
};

UENUM(BlueprintType)
enum class ETileType : uint8
{
	Wall,
	Empty,
	Pellet,
	PowerPellet,
	Teleport,
	Tunnel,
	HouseDoor
};

UENUM(BlueprintType)
enum class EPacmanGameState : uint8
{
	Idle,
	Started,
	Paused,
	Won,
	Lost
};


UENUM(BlueprintType)
enum class EGhostType : uint8
{
	Blinky,
	Pinky,
	Inky,
	Clyde
};

UENUM(BlueprintType)
enum class EGhostState : uint8
{
	None,
	Scatter,
	Chase,
	Frightened,
};

UENUM(BlueprintType)
enum class EGhostInstanceState : uint8
{
	None,
	Eaten,
	InHouse
};

UENUM(BlueprintType)
enum class EPacmanCollectibles : uint8
{
	Pellet,
	PowerPellet,
};
UENUM(BlueprintType)
enum class ESpeedOfState : uint8
{
	Base,
	Frightened,
	Eaten,
};