#pragma once
#include "CoreMinimal.h"
#include "CharacterTypes.generated.h"



UENUM(BlueprintType)
enum class ECharacterResource : uint8
{
	None,
	Health,
	Energy
};
UENUM(BlueprintType)
enum class EEntityType : uint8
{
	None,
	Owner,
	Ally,
	Enemy
};
UENUM(BlueprintType)
enum class EEntityState : uint8
{
	None,
	Idle,
	Moving,
	Dead,
	Casting,
	Attacking,
	ObtainingTarget
};	

DECLARE_DELEGATE_OneParam(FOnStateRequest, EEntityState);