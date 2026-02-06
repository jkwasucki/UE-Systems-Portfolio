// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/CharacterVFXComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Main/Character/Derived/MainCharacter.h"
// Sets default values for this component's properties
UCharacterVFXComponent::UCharacterVFXComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCharacterVFXComponent::PlayVFX(FVFXData& VFXData, const FGuid& Identifier)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	
	USkeletalMeshComponent* Mesh = Character->GetMesh();
	
	UNiagaraComponent* NiagaraComp = nullptr;
	
	if (!VFXData.Niagara) return;

	if (VFXData.bIsAttached)
	{
		NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			VFXData.Niagara,
			Mesh,
			VFXData.SocketName,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);
	}
	else
	{
		const FTransform SocketTransform = Mesh->GetSocketTransform(VFXData.SocketName);
		
		NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		   GetWorld(),
		   VFXData.Niagara,
		   SocketTransform.GetLocation(),
		   VFXData.SocketName == NAME_None ? FRotator::ZeroRotator : SocketTransform.GetRotation().Rotator(),
		   VFXData.SocketName == NAME_None ? FVector(1) : SocketTransform.GetScale3D(),
		   true
		);
	}
	
	ActiveVisualEffects.Add(Identifier,NiagaraComp);
}

void UCharacterVFXComponent::StopVFX(const FGuid& Identifier)
{
	TArray<UNiagaraComponent*> Components;
	ActiveVisualEffects.MultiFind(Identifier, Components);

	for (UNiagaraComponent* Comp : Components)
	{
		if (IsValid(Comp))
		{
			Comp->Deactivate();
		}
	}

	ActiveVisualEffects.Remove(Identifier);
}

