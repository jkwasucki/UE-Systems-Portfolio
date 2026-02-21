// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraComponent.h"
#include "Structs/FVFXData.h"
#include "CharacterVFXComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UCharacterVFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterVFXComponent();

protected:
	
	TMultiMap<FGuid, UNiagaraComponent*> ActiveVisualEffects;
public:	

	UFUNCTION()
	void PlayVFX(FVFXData VFX, const FGuid Identifier);
	
	UFUNCTION()
	void StopVFX(const FGuid& Identifier);
};
