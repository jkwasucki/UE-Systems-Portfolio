// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/MyGameInstance.h"

#include "Structs/FAnimationTags.h"

void UMyGameInstance::Init()
{
	Super::Init();
	
	FAnimationTags::Initialize();
}
