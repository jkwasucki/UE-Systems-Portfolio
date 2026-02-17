// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionInstance.generated.h"

/**
 * 
 */
class UInteractionDefinition;
class UInteractionAction;

UCLASS()
class CLONE1_API UInteractionInstance : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInteractionDefinition> Definition;
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UInteractionAction>> InstancedActions;
	
public:
	UPROPERTY()
	TWeakObjectPtr<AActor> Instigator;
	UPROPERTY()
	TWeakObjectPtr<AActor> Target;
	
protected:
	
	UFUNCTION()
	void InstantiateActions();

public:
	UFUNCTION()
	void Init(AActor* InInstigator, AActor* InTarget, UInteractionDefinition* InDefinition);
};
