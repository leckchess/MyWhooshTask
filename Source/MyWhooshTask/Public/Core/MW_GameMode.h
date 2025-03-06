// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "MW_GameMode.generated.h"

class UStaticMesh;
class UAnimInstance;
class UDataTable;

USTRUCT(BlueprintType)
struct FCharacterPawnsData : public FTableRowBase
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, Category = "Tag")
	FGameplayTag PawnTag;

	UPROPERTY(EditAnywhere, Category = "Pawn")
	TSubclassOf<APawn> Pawn;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TSubclassOf<UStaticMesh> OverrideMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	bool bOverridePawnTransform = false;

	UPROPERTY(EditAnywhere, Category = "Mesh", meta = (ToolTip = "Relative", EditCondition = "bOverridePawnTransform", EditConditionHides))
	FVector OverrideMeshLocation;

	UPROPERTY(EditAnywhere, Category = "Mesh", meta = (ToolTip = "Relative", EditCondition = "bOverridePawnTransform", EditConditionHides))
	FVector OverrideMeshRotation;

	UPROPERTY(EditAnywhere, Category = "Mesh", meta = (EditCondition = "bOverridePawnTransform", EditConditionHides))
	FVector OverideMeshScale;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TSubclassOf<UAnimInstance> OverrideAnimationClass;

	UPROPERTY(EditAnywhere, Category = "Camera")
	bool bOverrideCameraSetup = false;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (EditCondition = "bOverrideCameraSetup", EditConditionHides))
	float OverrideCameraTargetArmLength;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (EditCondition = "bOverrideCameraSetup", EditConditionHides))
	FVector OverrideCameraSocketOffset;
};

UCLASS(minimalapi)
class AMW_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMW_GameMode();

protected:

	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;

private:
	bool LoadPawnsData();
	FCharacterPawnsData* GetRandomPawnData();

public:
	UPROPERTY(EditAnywhere, Category = "Character Pawns")
	UDataTable* CharactersPawnsDataTable;

private:
	TMap<FGameplayTag, FCharacterPawnsData*> CharacterPawns;

	TSubclassOf<APawn> CachedDefaultPawnClass;
};

