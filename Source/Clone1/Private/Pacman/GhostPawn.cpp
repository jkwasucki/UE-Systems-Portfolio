// Fill out your copyright notice in the Description page of Project Settings.


#include "PacMan/GhostPawn.h"
#include "PacMan/PacmanGame.h"

AGhostPawn::AGhostPawn()
{
	PrimaryActorTick.bCanEverTick = true;

}


void AGhostPawn::BeginPlay()
{
	Super::BeginPlay();
	//
	// if (!bIsPacman && GhostType != EGhostType::Blinky)
	// {
	// 	GhostState = EGhostState::InHouse;
	// }
}


void AGhostPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGhostPawn::OnExitReached()
{
	Super::OnExitReached();
}

void AGhostPawn::OnSpawnReached()
{
	Super::OnSpawnReached();
	SetFlipbook(SourceFlipbook);
	Revive();
}

void AGhostPawn::RestartEntity()
{
	Super::RestartEntity();
	
	bIsActive = false;
	SetFlipbook(SourceFlipbook);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void AGhostPawn::SetupEntity(APacmanGame* GI)
{
	Super::SetupEntity(GI);
		
	if (!GridMoverComponent) return;
	
	GI->OnChangeGhostStateDelegate.RemoveAll(this);
	GI->OnChangeGhostStateDelegate.AddDynamic(this, &AGhostPawn::SetState);
}




void AGhostPawn::SetState(EGhostState State)
{
	if (GhostInstanceState == EGhostInstanceState::InHouse || GhostInstanceState == EGhostInstanceState::Eaten) return;
	
	switch (State)
	{
		case EGhostState::Frightened:FearState();break;
		case EGhostState::Scatter:ScatterChaseState();break;
		case EGhostState::Chase:ScatterChaseState();break;
	}
}

void AGhostPawn::Activate()
{
	bIsActive = true;
	GridMoverComponent->ToggleMovement(true);
}

void AGhostPawn::Eliminate()
{
	if (GhostInstanceState == EGhostInstanceState::Eaten) return;
	
	GhostInstanceState = EGhostInstanceState::Eaten;
	GridMoverComponent->OnBecameEaten();
	SetSpeed(ESpeedOfState::Eaten);
	SetFlipbook(EatenStateFlipbook);
}

void AGhostPawn::Revive()
{
	
	FTimerDelegate TimerDelegate;
	
	TimerDelegate.BindLambda([this]()
	{
			ToggleMovement(true);
	}
	);
	
	GetWorld()->GetTimerManager().SetTimer(
		ReviveTimerHandle,
		TimerDelegate,
		ReviveDuration,
		false
	);
}


void AGhostPawn::FearState()
{
	SetSpeed(ESpeedOfState::Frightened);
	SetFlipbook(FrightenedStateFlipbook);
}

void AGhostPawn::ScatterChaseState()
{
	SetSpeed(ESpeedOfState::Base);
	SetFlipbook(SourceFlipbook);
}


