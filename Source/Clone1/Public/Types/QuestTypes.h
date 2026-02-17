#pragma once
#include "CoreMinimal.h"
#include "QuestTypes.generated.h"



UENUM(BlueprintType)
enum class EQuestState : uint8
{
	None,
	Active,
	Completed,
	Abandoned,
	Failed,
};

UENUM(BlueprintType)
enum class EConditionLogic : uint8
{
	None,
	XorY,
	XandY,
};