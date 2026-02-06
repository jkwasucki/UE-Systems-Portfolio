#pragma once
#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "FVFXData.generated.h"
USTRUCT(BlueprintType)
struct FVFXData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName SocketName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UNiagaraSystem* Niagara;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool bIsAttached;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float Duration;
};
