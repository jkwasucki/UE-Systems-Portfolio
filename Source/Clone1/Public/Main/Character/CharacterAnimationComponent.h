// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs/FAnimationData.h"
#include "Components/ActorComponent.h"
#include "Main/Character/CharacterMoverComponent.h"
#include "CharacterAnimationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UCharacterAnimationComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UCharacterAnimationComponent();
protected:
	UPROPERTY()
	UCharacterMoverComponent* CharacterMovementComponent = nullptr;
	
	
	UPROPERTY()
	UAnimMontage* CurrentAnimMontage = nullptr;
	
	// ROTATION
	UPROPERTY()
	bool bShouldRotate = false;
	UPROPERTY()
	FVector OverwriteRotationVector;
	
	UPROPERTY()
	FTimerHandle PoseHoldTimerHandle;
	UPROPERTY()
	bool bIsPlayingMontage = false;
	UPROPERTY()
	FAnimationData PendingAnimData;

public:	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TArray<FAnimationData> Animations;				// AVAILABLE ANIMATIONS FOR THIS CHARACTER
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUpperBodyAnimActive = false;			// SWITCH BETWEEN ANIM LAYERS
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION()
	void PlayAnimation(FAnimationData AnimData);
	UFUNCTION()
	void PlayMontageInternal();
	UFUNCTION()
	void PlayAnimationByTag(FGameplayTag Tag);
	UFUNCTION()
	void RotateTowards(FVector Vector);
	UFUNCTION()
	void StopCurrentMontage();
	UFUNCTION()
	void SetCharacterMovementComponentLink(UCharacterMoverComponent* Comp);
	UFUNCTION()
	void RequestRotate(FVector Vector);
};
