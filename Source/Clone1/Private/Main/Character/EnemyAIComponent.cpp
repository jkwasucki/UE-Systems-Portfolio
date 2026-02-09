// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/EnemyAIComponent.h"

#include "Main/Character/Derived/EnemyCharacter.h"

// Sets default values for this component's properties
UEnemyAIComponent::UEnemyAIComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	AIAttackTriggerZone = CreateDefaultSubobject<USphereComponent>("AIAttackTriggerZone");
	AIAttackTriggerZone->SetCollisionObjectType(ECC_GameTraceChannel3); // AITrigger channel
	AIAttackTriggerZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AIAttackTriggerZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	AIAttackTriggerZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	AIAttackTriggerZone->OnComponentBeginOverlap.AddDynamic(this, &UEnemyAIComponent::OnFoeInRange);
	AIAttackTriggerZone->OnComponentEndOverlap.AddDynamic(this, &UEnemyAIComponent::OnFoeOutOfRange);
}


void UEnemyAIComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (AActor* Owner = GetOwner())
	{
		if (USceneComponent* Root = Owner->GetRootComponent())
		{
			AIAttackTriggerZone->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

// Called every frame
void UEnemyAIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	HandleState();
	if (IsFoeInRange())
	{
		FaceFoe(DeltaTime);
	}
}

void UEnemyAIComponent::Initialize(ABaseCharacter* InBaseCharacter)
{
	if (InBaseCharacter)
	{
		BaseCharacter = InBaseCharacter;
	}
}



AActor* UEnemyAIComponent::GetClosestFoe()
{
	if (!IsValid(BaseCharacter) || FoesInRange.Num() == 0)
		return nullptr;

	AActor* ClosestFoe = nullptr;
	float ClosestDistanceSq = FLT_MAX;

	const FVector MyLocation = BaseCharacter->GetActorLocation();

	for (AActor* Foe : FoesInRange)
	{
		if (!IsValid(Foe))
			continue;

		const float DistSq = FVector::DistSquared(MyLocation, Foe->GetActorLocation());

		if (DistSq < ClosestDistanceSq)
		{
			ClosestDistanceSq = DistSq;
			ClosestFoe = Foe;
		}
	}

	return ClosestFoe;
}

void UEnemyAIComponent::HandleState()
{
	if (!IsValid(BaseCharacter)) return;
	EEntityState NewState = BaseCharacter->GetState();
	switch (NewState)
	{
		case EEntityState::Idle:
		{
			if (IsFoeInRange())
				BaseCharacter->SetState(EEntityState::Attacking);
			break;
		}
		case EEntityState::Attacking:
		{
			TryAttacking();
				
			if (!IsFoeInRange())
			{
				BaseCharacter->SetState(EEntityState::Idle);
			}
		}
	}
}

bool UEnemyAIComponent::IsFoeInRange()
{
	return FoesInRange.Num() > 0;
}

void UEnemyAIComponent::TryAttacking()
{
	AActor* Closest = GetClosestFoe();
	if (!IsValid(Closest)) return;
	
	
	// GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Green,TEXT("ATTACKING!!"));
	if (AEnemyCharacter* GameplayCharacter = Cast<AEnemyCharacter>(BaseCharacter))
	{
		UAbilitySystemComponent* AbilitySystemComponent = ICharacterEffectReceiverInterface::Execute_GetAbilitySystemComponent(GetOwner());
		if (AbilitySystemComponent->GetGrantedAbilities().Num() == 0) return;
		
		UAbilityData* Ability = AbilitySystemComponent->GetGrantedAbilities()[0];
		AbilitySystemComponent->TryUseAbility(Ability);
		
	}
}

void UEnemyAIComponent::FaceFoe(float DeltaTime)
{
	if (!IsValid(BaseCharacter))
		return;

	AActor* Closest = GetClosestFoe();
	if (!IsValid(Closest))
		return;

	FVector ToTarget = Closest->GetActorLocation() - BaseCharacter->GetActorLocation();
	FRotator TargetRotation = ToTarget.Rotation();

	FRotator CurrentRotation = BaseCharacter->GetActorRotation();

	FRotator NewRotation = FMath::RInterpTo(
		CurrentRotation,
		TargetRotation,
		DeltaTime,
		5.0f   // Rotation speed (adjust as needed)
	);

	BaseCharacter->SetActorRotation(NewRotation);
}
void UEnemyAIComponent::OnFoeInRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABaseCharacter* Foe = Cast<ABaseCharacter>(OtherActor))
	{
		if (Foe->EntityType == EEntityType::Ally || Foe->EntityType == EEntityType::Owner)
		{
			if (!FoesInRange.Contains(OtherActor))
				FoesInRange.Add(OtherActor);
		}
	}
}

void UEnemyAIComponent::OnFoeOutOfRange(  UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (ABaseCharacter* Foe = Cast<ABaseCharacter>(OtherActor))
	{
		if (Foe->EntityType == EEntityType::Ally || Foe->EntityType == EEntityType::Owner)
		{
			if (FoesInRange.Contains(OtherActor))
				FoesInRange.Remove(OtherActor);
		}
	}
}