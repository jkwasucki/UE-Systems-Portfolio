// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/CharacterAnimationComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Main/Character/Derived/MainCharacter.h"


UCharacterAnimationComponent::UCharacterAnimationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UCharacterAnimationComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(true);
}

void UCharacterAnimationComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCharacterAnimationComponent::PlayAnimation(FAnimationData AnimData)
{
	if (!AnimData.AnimSequence) return;

	bIsUpperBodyAnimActive =
		AnimData.AnimationMode == EAnimationBodyPart::Upperbody;
	PendingAnimData = AnimData;
	
	// Let AnimGraph update first
	GetWorld()->GetTimerManager().SetTimerForNextTick(
		FTimerDelegate::CreateUObject(
			this,
			&UCharacterAnimationComponent::PlayMontageInternal
		)
	);
}
void UCharacterAnimationComponent::PlayMontageInternal()
{
	ABaseCharacter* Char = Cast<ABaseCharacter>(GetOwner());
	if (!Char) return;

	USkeletalMeshComponent* MeshComp = Char->GetMesh();
	if (!MeshComp) return;

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (!AnimInstance) return;

	const float AnimLength = PendingAnimData.AnimSequence->GetPlayLength();
	const float PlayRate =
		PendingAnimData.TweakDuration > KINDA_SMALL_NUMBER
			? AnimLength / PendingAnimData.TweakDuration
			: 1.f;

	const float BlendOutTime = PendingAnimData.bKeepPose ? 0.f : 0.5f;

	UAnimMontage* PlayedMontage =
		AnimInstance->PlaySlotAnimationAsDynamicMontage(
			PendingAnimData.AnimSequence,
			TEXT("DefaultSlot"),
			0.1f,
			BlendOutTime,
			PlayRate
		);

	if (!PlayedMontage) return;

	bIsPlayingMontage = true;
	CurrentAnimMontage = PlayedMontage;
}

void UCharacterAnimationComponent::PlayAnimationByTag(FGameplayTag Tag)
{
	for (FAnimationData& Anim : Animations)
	{
		if (Anim.TagName == Tag)
		{
			PlayAnimation(Anim);
			return;
		}
	}
}

void UCharacterAnimationComponent::RotateTowards(FVector Vector)
{
	if (Vector.IsNearlyZero())
		return;
	
	AMainCharacter* Char = Cast<AMainCharacter>(GetOwner());
	if (!Char) return;

	Char->bUseControllerRotationYaw = false;

	if (UCharacterMovementComponent* MoveComp = Char->GetCharacterMovement())
	{
		MoveComp->bOrientRotationToMovement = false;
	}

	

	// Current rotation
	const FRotator CurrentRotation = Char->GetActorRotation();

	// Desired rotation (only yaw)
	const FRotator TargetRotation = FRotationMatrix::MakeFromX(Vector).Rotator();
	const FRotator DesiredRotation(0.f, TargetRotation.Yaw, 0.f);

	// Smooth interpolation
	const float RotationSpeed = 8.f; // tweak this
	const float DeltaTime = GetWorld()->GetDeltaSeconds();

	const FRotator NewRotation = FMath::RInterpTo(
		CurrentRotation,
		DesiredRotation,
		DeltaTime,
		RotationSpeed
	);

	Char->SetActorRotation(NewRotation);
	
}

void UCharacterAnimationComponent::SetCharacterMovementComponentLink(UCharacterMoverComponent* Comp)
{
	if (!Comp) return;
	CharacterMovementComponent = Comp;
}

void UCharacterAnimationComponent::RequestRotate(FVector Vector)
{
	OverwriteRotationVector = Vector;
	bShouldRotate = true;
}

void UCharacterAnimationComponent::StopCurrentMontage()
{
	AMainCharacter* Char = Cast<AMainCharacter>(GetOwner());
	if (!Char) return;
	
	USkeletalMeshComponent* MeshComp = Char->GetMesh();
	if (!MeshComp) return;

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (!AnimInstance) return;
	
	AnimInstance->Montage_Stop(0.3,CurrentAnimMontage);
	
	if (bIsUpperBodyAnimActive)
		bIsUpperBodyAnimActive = false;
}
