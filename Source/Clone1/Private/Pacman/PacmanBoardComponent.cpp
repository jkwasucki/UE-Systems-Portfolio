// Fill out your copyright notice in the Description page of Project Settings.


#include "PacMan/PacmanBoardComponent.h"
#include "Pacman/PacmanTypes.h"
#include "PacMan/PacmanGame.h"

UPacmanBoardComponent::UPacmanBoardComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PelletSprites = CreateDefaultSubobject<UPaperGroupedSpriteComponent>("PelletSprites");
	PelletSprites->SetupAttachment(this);
}

int32 UPacmanBoardComponent::GetPelletCount() const
{
	return PelletCount;
}

void UPacmanBoardComponent::InitializeBoard(APacmanGame* GI)
{
	if (!GameInstance)
		GameInstance = GI;
	
	Tiles.SetNum(Width * Height);
	
	InitFromAscii();
	SpawnPellets();
}

void UPacmanBoardComponent::SpawnPellets()
{
	PelletSprites->ClearInstances();
	PelletCount = 0;
	
	for (int32 y = 0; y < Height; y++)
	{
			for (int32 x = 0; x < Width; x++)
			{
				FVector Pos = GridToWorld(FIntPoint(x, y));
				ETileType TT = Tiles[IndexOfTileByCoordinate(x,y)];
				if (TT == ETileType::Pellet || TT == ETileType::PowerPellet)
				{
					const FRotator PelletRotation(0.f, 0.f, 90.f);
					FTransform InstanceTransform;
					InstanceTransform.SetLocation(Pos);
					InstanceTransform.SetRotation(PelletRotation.Quaternion());
					InstanceTransform.SetScale3D(FVector(TT== ETileType::PowerPellet ? 2.f : 1.f));
					
					

					int32 PelletIndex = PelletSprites->AddInstance(
						InstanceTransform,
						TT == ETileType::Pellet ? PelletSprite : PowerPelletSprite,
						true
					);
					
					PelletCount++;
				}
			}
	}
}

void UPacmanBoardComponent::RemovePellet(FIntPoint GridPos)
{
	ETileType CollectedType = ETileType::Pellet;
	int32 Index = IndexOfTileByCoordinate(GridPos.X,GridPos.Y);
	if (Tiles.IsValidIndex(Index))
	{
		CollectedType = Tiles[Index];
		Tiles[Index] = ETileType::Empty;
		OnCollected.Broadcast(CollectedType == ETileType::Pellet ? EPacmanCollectibles::Pellet : EPacmanCollectibles::PowerPellet);
		SpawnPellets();
	}
	
	if (PelletCount == 0)
	{
		OnPelletsCollectedDelegate.Broadcast();
	}
	
}

bool UPacmanBoardComponent::GetTeleportExit(const FIntPoint& InPoint, FIntPoint& OutPoint) const
{
	OutPoint = FIntPoint(0,0);
	
	if (InPoint == FIntPoint(0, 16))
		OutPoint =  FIntPoint(27, 16);

	if (InPoint == FIntPoint(27, 16))
		OutPoint = FIntPoint(0, 16);
	
	if (OutPoint == FIntPoint(27, 16) || OutPoint == FIntPoint(0, 16))
		return true;
	else
		return false;
}

void UPacmanBoardComponent::TeleportGhost(APacmanEntity* Entity,FIntPoint& InPoint)
{
	
//
}


void UPacmanBoardComponent::InitFromAscii()
{
	for (int32 y = 0; y < Height; y++)
	{
		for (int32 x = 0; x < Width; x++)
		{
			int32 SourceY = Height - 1 - y;
			TCHAR C = Maze[SourceY][x];
			
			switch (C)
			{
			case '#': Tiles[IndexOfTileByCoordinate(x,y)] = ETileType::Wall; break;
			case '.': Tiles[IndexOfTileByCoordinate(x,y)] = ETileType::Pellet; break;
			case 'o': Tiles[IndexOfTileByCoordinate(x,y)] = ETileType::PowerPellet; break;
			case ' ': Tiles[IndexOfTileByCoordinate(x,y)] = ETileType::Empty; break;
			case '$': Tiles[IndexOfTileByCoordinate(x,y)] = ETileType::Teleport; break;
			case '%': Tiles[IndexOfTileByCoordinate(x,y)] = ETileType::Tunnel; break;
			case '@': Tiles[IndexOfTileByCoordinate(x,y)] = ETileType::HouseDoor; break;
			}
		}
	}
}


FVector UPacmanBoardComponent::GridToWorld(const FIntPoint& GridPos)
{
	if (!GameInstance)
	{
		return FVector::ZeroVector;
	};
	
	const float HalfTile = TileSize * 0.5f;
	
	const float WorldX = GridPos.X * TileSize + HalfTile;
	const float WorldY = GridPos.Y * TileSize + HalfTile;
	
	return GameInstance->GetActorLocation() + FVector(WorldX, WorldY,0.1f);
}

void UPacmanBoardComponent::Reset()
{
	if (GameInstance)
		InitializeBoard(GameInstance);
	
}

const TCHAR* UPacmanBoardComponent::Maze[31] =
{
	TEXT("############################"),
	TEXT("#o...........##...........o#"),
	TEXT("#.####.#####.##.#####.####.#"),
	TEXT("#.####.#####.##.#####.####.#"),
	TEXT("#.####.#####.##.#####.####.#"),
	TEXT("#..........................#"),
	TEXT("#.####.##.########.##.####.#"),
	TEXT("#.####.##.########.##.####.#"),
	TEXT("#......##....##....##......#"),
	TEXT("######.##### ## #####.######"),
	TEXT("     #.##### ## #####.#     "),
	TEXT("     #.##          ##.#     "),
	TEXT("######.## ###@@### ##.######"),
	TEXT("######.## #      # ##.######"),
	TEXT("$    %    #      #    %    $"),
	TEXT("######.## #      # ##.######"),
	TEXT("######.## ######## ##.######"),
	TEXT("     #.##          ##.#     "),
	TEXT("     #.## ######## ##.#     "),
	TEXT("######.## ######## ##.######"),
	TEXT("#............##............#"),
	TEXT("#.####.#####.##.#####.####.#"),
	TEXT("#.####.#####.##.#####.####.#"),
	TEXT("#...##................##...#"),
	TEXT("###.##.##.########.##.##.###"),
	TEXT("###.##.##.########.##.##.###"),
	TEXT("#......##....##....##......#"),
	TEXT("#.##########.##.##########.#"),
	TEXT("#.##########.##.##########.#"),
	TEXT("#o........................o#"),
	TEXT("############################")
};


	


