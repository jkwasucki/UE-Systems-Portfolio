#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FAnimationTags
{
	static FGameplayTag Animation_CharacterHit;
	static FGameplayTag Animation_CharacterDeath;
	static FGameplayTag Animation_CharacterResurrect;
	///
	
	
	static void Initialize();
};
