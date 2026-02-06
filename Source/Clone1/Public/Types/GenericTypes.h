#pragma once
#include "CoreMinimal.h"
#include "GenericTypes.generated.h"


UENUM(BlueprintType)
enum class EMoveDirection : uint8
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

UENUM(BlueprintType)
enum class EAnimationBodyPart : uint8
{
	Fullbody,
	Upperbody,
};
