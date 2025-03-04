// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core\MW_GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 *
 */
UCLASS()
class MYWHOOSHTASK_API ALobbyGameMode : public AMW_GameMode
{
	GENERATED_BODY()

public:
	void PostLogin(APlayerController* NewPlayer) override;

	void Logout(AController* Exiting) override;

private:
	void StartGame();

private:

	FTimerHandle StartGame_Handle;
	uint32 PlayersNumber = 0;
};
