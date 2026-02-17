// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystem/HUD/QuestInstanceWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "QuestSystem/Instances/QuestInstance.h"
#include "QuestDebugWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UQuestDebugWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UQuestInstanceWidget> QuestInstanceWidgetClass;
	
	UPROPERTY()
	TArray<UQuestInstanceWidget*> QuestWidgets;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ActiveCount;
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* QuestList;
	
	
	UFUNCTION()
	void DisplayQuest(UQuestInstance* NewQuest);
	UFUNCTION()
	void RemoveQuest(UQuestInstance* Quest);

};
