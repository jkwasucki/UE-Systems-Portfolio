// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionSystem/Definitions/InteractionDefinition.h"
#include "InteractionSystem/Instances/InteractionInstance.h"

void UInteractionInstance::Init(AActor* InInstigator, AActor* InTarget, UInteractionDefinition* InDefinition)
{
	if (!IsValid(InInstigator) || !IsValid(InTarget) || !IsValid(InDefinition)) return;
	
	Instigator = InInstigator;
	Target = InTarget;
	Definition = InDefinition;
	
	InstantiateActions();
}

void UInteractionInstance::InstantiateActions()
{
	for (TSubclassOf<UInteractionAction> Action : Definition->Actions)
	{
		if (UInteractionAction* NewAction = NewObject<UInteractionAction>(this, Action))
		{
			InstancedActions.Add(NewAction);
			NewAction->Execute(this);
		}
	}
}
