#include "G:\UnrealProjects\Clone1\Intermediate\Build\Win64\x64\Clone1Editor\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.Cpp20.InclOrderUnreal5_3.h"

#include "GameplayTagsManager.h"
#include "FAnimationTags.h"


FGameplayTag FAnimationTags::Animation_CharacterHit;
FGameplayTag FAnimationTags::Animation_CharacterDeath;
FGameplayTag FAnimationTags::Animation_CharacterResurrect;

void FAnimationTags::Initialize()
{
	auto& Manager = UGameplayTagsManager::Get();
	
	Animation_CharacterHit = Manager.RequestGameplayTag(FName("Animations.Character.Hit"));
	Animation_CharacterDeath = Manager.RequestGameplayTag(FName("Animations.Character.Death"));
	Animation_CharacterResurrect = Manager.RequestGameplayTag(FName("Animations.Character.Resurrect"));
}
