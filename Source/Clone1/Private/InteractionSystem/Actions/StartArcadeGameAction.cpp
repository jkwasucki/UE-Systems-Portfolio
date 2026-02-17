// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionSystem/Instances/InteractionInstance.h"
#include "InteractionSystem/Actions/StartArcadeGameAction.h"

#include "Actors/ArcadeMachine.h"
#include "Main/Character/Derived/MainCharacter.h"

void UStartArcadeGameAction::Execute(UInteractionInstance* Instance)
{
	if (!IsValid(Instance) || !Instance->Instigator.IsValid()) return;
	
	AActor* Instigator = Instance->Instigator.Get();
	AMainCharacter* Character = Cast<AMainCharacter>(Instigator);
	AActor* Target = Instance->Target.Get();
	AArcadeMachine* ArcadeMachine = Cast<AArcadeMachine>(Target);
	
	if (!Character) return;
	if (!ArcadeMachine) return;
	if (!ArcadeMachine->PacmanGame) return;
	
	
	AMainPlayerController* PC = Character->GetController<AMainPlayerController>();
	if (!PC) return;
	
	IInteractableInterface::Execute_OnInteractionExecuted(ArcadeMachine);
	PC->StartPacmanGame(ArcadeMachine);
}
