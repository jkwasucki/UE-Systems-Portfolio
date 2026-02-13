// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PacmanEntity.h"
#include "PacMan/PacmanGame.h"
#include "GameFramework/Pawn.h"
#include "Main/Character/CharacterInputComponent.h"
#include "PacmanPawn.generated.h"

class APacmanGame;

UCLASS()
class CLONE1_API APacmanPawn : public APacmanEntity
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APacmanPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual bool IsPacman() const override {return true;}
	
	
public:	
	
	UPROPERTY(EditAnywhere)
	UPaperFlipbook* Flipbook_Death;
	UPROPERTY(EditAnywhere)
	UPaperFlipbook* Flipbook_Left;
	UPROPERTY(EditAnywhere)
	UPaperFlipbook* Flipbook_Right;
	UPROPERTY(EditAnywhere)
	UPaperFlipbook* Flipbook_Up;
	UPROPERTY(EditAnywhere)
	UPaperFlipbook* Flipbook_Down;
	
	virtual void RestartEntity() override;
	
	UFUNCTION()
	void Eliminate();
	UFUNCTION()
	void SetFlipbookByDir(EMoveDirection Dir);
	UFUNCTION()
	void HandleInputDelegates(UCharacterInputComponent* CharacterInput);

	virtual void SetupEntity(APacmanGame* GI) override;
};
