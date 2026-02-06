#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Types/GenericTypes.h"
#include "FAnimationData.generated.h"

USTRUCT(BlueprintType)
struct FAnimationData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag TagName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	EAnimationBodyPart AnimationMode;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bBlocksMovement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bLoop;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TweakDuration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequence* AnimSequence;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bKeepPose;
};
