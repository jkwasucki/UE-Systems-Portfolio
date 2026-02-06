// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InteractableComponent.h"
#include "PacMan/PacmanGame.h"
#include "PacMan/PacmanHUD.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "ArcadeMachine.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuit);
class APacmanGame;
UCLASS()
class CLONE1_API AArcadeMachine : public AActor,public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArcadeMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY()
	bool bIsActive = false;
	
	
	UPROPERTY()
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* Collision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInteractableComponent* InteractableComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> PacmanHUDClass;
	
	UPROPERTY(VisibleAnywhere, Category="UI")
	UWidgetComponent* PacmanHUDComponent;
	virtual void OnConstruction(const FTransform& Transform) override;
	UFUNCTION()
	void OnInteract(ACharacter* Char);
	
	UFUNCTION()
	void FocusMachine();
	
public:	
	UPROPERTY()
	UInputHandlerComponent* InputHandlerComponent = nullptr;
	
	
	FOnQuit OnQuitDelegate;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Arcade")
	APacmanGame* PacmanGame = nullptr;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	APacmanGame* GetPacmanGame();
	UFUNCTION()
	void SetupArcade(UInputHandlerComponent* inInputHandlerComponent);
	UFUNCTION()
	void Quit();
	UFUNCTION()
	void Start();
};
