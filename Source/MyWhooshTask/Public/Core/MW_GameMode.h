// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MW_GameMode.generated.h"

class UStaticMesh;
class UAnimInstance;
class UDataTable;
struct FCharacterPawnsData;

UCLASS(minimalapi)
class AMW_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMW_GameMode();

protected:

	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;

	void PostLogin(APlayerController* NewPlayer) override;

	void Logout(AController* Exiting) override;

private:
	bool LoadPawnsData();
	FCharacterPawnsData* GetRandomPawnData();
	/** Get datatable dynamically in case using a gamemode class instead of bp */
	bool TryGetCharactersPawnData();

	void StartGame();

public:
	UPROPERTY(EditAnywhere, Category = "Character Pawns")
	UDataTable* CharactersPawnsDataTable;

private:
	FTimerHandle StartGame_Handle;
	uint32 PlayersNumber = 0;
};

