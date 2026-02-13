// Fill out your copyright notice in the Description page of Project Settings.


#include "PacMan/PacmanPawn.h"
#include "Types/GenericTypes.h"
#include "PaperFlipbookComponent.h"
#include "Main/Character/Derived/MainCharacter.h"
#include "Main/PlayerController/InputHandlerComponent.h"
#include "PacMan/PacmanGame.h"

APacmanPawn::APacmanPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APacmanPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void APacmanPawn::HandleInputDelegates(UCharacterInputComponent* CharacterInput)
{
	if (CharacterInput)
	{
		CharacterInput->OnArrowPressDelegate.AddDynamic(GridMoverComponent, &UGridMoverComponent::SetDirection);
	}
}

void APacmanPawn::SetupEntity(APacmanGame* GI)
{
	Super::SetupEntity(GI);
	
	if (!GridMoverComponent) return;
	
	GridMoverComponent->OnDirectionValidatedDelegate.RemoveAll(this);
	GridMoverComponent->OnDirectionValidatedDelegate.AddDynamic(this,&APacmanPawn::SetFlipbookByDir);
	
	if (GI->PlayerController)
	{
		AMainCharacter* MC = GI->PlayerController->GetMainCharacter();
		if (MC)
		{
			HandleInputDelegates(MC->GetCharacterInput());
			SetFlipbookByDir(GridMoverComponent->CurrentDirection);
		}
	}
	

}


void APacmanPawn::RestartEntity()
{
	Super::RestartEntity();
	
	SetFlipbookByDir(GridMoverComponent->CurrentDirection);
	SpriteComponent->SetLooping(true);
	SpriteComponent->PlayFromStart();
}

void APacmanPawn::Eliminate()
{
	SpriteComponent->SetFlipbook(Flipbook_Death);
	SpriteComponent->SetLooping(false);
	SpriteComponent->PlayFromStart();
}

void APacmanPawn::SetFlipbookByDir(EMoveDirection Dir)
{
	UPaperFlipbook* Flipbook = nullptr;
	
	switch (Dir)
	{
	case EMoveDirection::LEFT: Flipbook = Flipbook_Right; break;
	case EMoveDirection::RIGHT: Flipbook = Flipbook_Left; break;
	case EMoveDirection::UP: Flipbook = Flipbook_Down; break;
	case EMoveDirection::DOWN: Flipbook = Flipbook_Up; break;
	}
	
	if (SpriteComponent->GetFlipbook() == Flipbook) return;
	
	SpriteComponent->SetFlipbook(Flipbook);
	
}




