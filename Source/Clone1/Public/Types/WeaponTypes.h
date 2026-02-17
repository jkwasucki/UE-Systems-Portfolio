#pragma once
#include "CoreMinimal.h"
#include "WeaponTypes.generated.h"


UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
	Automatic,
	SemiAutomatic,
	Burst,
	
	Count UMETA(Hidden)
};
UENUM(BlueprintType)
enum class EWeaponInputCommand : uint8
{
	Auto,
	FirePressed,
	FireReleased,
	FireModeChanged,
};

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Idle,
	Firing,
	Reloading,
	//.................
};