// Copyright Huda Rasmey. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTagContainer.h"
#include "MW_PlayerState.generated.h"

/**
 *
 */
UCLASS()
class MYWHOOSHTASK_API AMW_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AMW_PlayerState();

	UPROPERTY(Replicated)
	uint32 PersistentPlayerID;

	UPROPERTY(Replicated)
	FGameplayTag PlayerPawnTag;

protected:
	virtual void BeginPlay() override;

};
