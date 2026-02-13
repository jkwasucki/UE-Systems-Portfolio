// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ArcadeMachine.h"


// Sets default values
AArcadeMachine::AArcadeMachine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	
	
	Collision = CreateDefaultSubobject<USphereComponent>("SphereCollider");
	Collision->SetupAttachment(RootComponent);
	
	
	InteractionComponent = CreateDefaultSubobject<UActorInteractionComponent>("InteractionComponent");
	
	PacmanHUDComponent = CreateDefaultSubobject<UWidgetComponent>("PacmanHUDComponent");
	PacmanHUDComponent->SetupAttachment(RootComponent);
	PacmanHUDComponent->SetWidgetSpace(EWidgetSpace::World);
	PacmanHUDComponent->SetDrawSize(FVector2D(300.f, 120.f));
	PacmanHUDComponent->SetRelativeLocation(FVector(0.f, 0.f, 140.f));
	PacmanHUDComponent->SetVisibility(false);
	

}

void AArcadeMachine::BeginPlay()
{
	Super::BeginPlay();
	
	if (!InteractionComponent) return;
	InteractionComponent->SetupComponent(this,Collision);
	if (PacmanHUDComponent)
	{
		UPacmanHUD* PacmanHUD = Cast<UPacmanHUD>(PacmanHUDComponent->GetUserWidgetObject());
		if (PacmanHUD && PacmanGame)
		{
			PacmanHUD->SetupHUD(PacmanGame);
		}
	}
}

void AArcadeMachine::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (PacmanHUDComponent && PacmanHUDClass)
	{
		PacmanHUDComponent->SetWidgetClass(PacmanHUDClass);
		PacmanHUDComponent->SetVisibility(true);
		PacmanHUDComponent->SetHiddenInGame(false);
	}
}

void AArcadeMachine::FocusMachine()
{
	if (!PacmanGame) return;
	if (AMainPlayerController* PC =
		Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PC->StartPacmanGame(this);
	}
}

// Called every frame
void AArcadeMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

APacmanGame* AArcadeMachine::GetPacmanGame()
{
	if (!IsValid(PacmanGame)) return nullptr;
	return PacmanGame;
}

void AArcadeMachine::SetupArcade(UInputHandlerComponent* inInputHandlerComponent)
{
	if (inInputHandlerComponent)
	{
		InputHandlerComponent = inInputHandlerComponent;
		
		InputHandlerComponent->OnTabDownDelegate.RemoveAll(this);
		InputHandlerComponent->OnTabDownDelegate.AddDynamic(this, &AArcadeMachine::Quit);
		InputHandlerComponent->OnSpaceDownDelegate.RemoveAll(this);
		InputHandlerComponent->OnSpaceDownDelegate.AddDynamic(this, &AArcadeMachine::Start);
	
		PacmanGame->InitializeGame();
	}
}

void AArcadeMachine::Interact_Implementation(ACharacter* Character)
{
	FocusMachine();
}

void AArcadeMachine::Quit()
{
	PacmanGame->QuitGame();
	InputHandlerComponent->OnTabDownDelegate.RemoveAll(this);
	InputHandlerComponent->OnSpaceDownDelegate.RemoveAll(this);
	InputHandlerComponent = nullptr;
	OnQuitDelegate.Broadcast();
	
	InteractionComponent->bIsInteractedWith = false;
}

void AArcadeMachine::Start()
{
	PacmanGame->Play();
}

