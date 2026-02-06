#pragma once
#include "CoreMinimal.h"
#include "Main/Character/AbilitySystemComponent.h"
#include "Structs/FCharacterEffect.h"
#include "CharacterEffectReciverInterface.generated.h"

UINTERFACE(BlueprintType)
class UCharacterEffectReceiverInterface : public UInterface
{
	GENERATED_BODY()
};

class ICharacterEffectReceiverInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void ApplyEffect(AActor* EffectOrigin, FCharacterEffect& Effects, FGuid SourceInstanceID);
	UFUNCTION(BlueprintNativeEvent)
	UAbilitySystemComponent* GetAbilitySystemComponent();
};
