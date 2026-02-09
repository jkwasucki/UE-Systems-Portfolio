#include "G:\UnrealProjects\Clone1\Intermediate\Build\Win64\x64\Clone1Editor\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.Cpp20.InclOrderUnreal5_3.h"
#include "AbilitySystem/Misc/AbilityProjectile.h"
#include "Main/Character/Derived/MainCharacter.h"
#include "AbilitySystem/Effects/ProjectileEffect.h"


bool UProjectileEffect::TryApplyEffect_Implementation(
	AActor* Instigator,
	FAbilityTargetData& TargetData,
	FGuid AbilityInstanceID)
{
	if (!Instigator)
		return false;

	UWorld* World = Instigator->GetWorld();
	if (!World)
		return false;

	FTransform SpawnTransform = Instigator->GetActorTransform();

	// Try to use a projectile spawn point if available
	if (AMainCharacter* Char = Cast<AMainCharacter>(Instigator))
	{
		if (Char->AbilityProjectileSpawnPoint)
		{
			SpawnTransform = Char->AbilityProjectileSpawnPoint->GetComponentTransform();
		}
	}

	FActorSpawnParameters Params;
	Params.Owner = Instigator;
	Params.Instigator = Cast<APawn>(Instigator);
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AAbilityProjectile* Projectile =
		World->SpawnActor<AAbilityProjectile>(
			ProjectileClass,
			SpawnTransform,
			Params
		);

	if (!Projectile)
		return false;

	ProjectileData.InflictedOnHitEffects = InflictedOnHitEffects;

	Projectile->InitializeProjectile(Instigator, ProjectileData, TargetData);

	return true;
}

