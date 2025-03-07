// Copyright Huda Rasmey. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameplayTagContainer.h"
#include "MW_GameStateBase.generated.h"

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
	USkeletalMesh* OverrideMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	bool bOverridePawnTransform = false;

	UPROPERTY(EditAnywhere, Category = "Mesh", meta = (ToolTip = "Relative", EditCondition = "bOverridePawnTransform", EditConditionHides))
	FVector OverrideMeshLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Mesh", meta = (ToolTip = "Relative", EditCondition = "bOverridePawnTransform", EditConditionHides))
	FVector OverrideMeshRotation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Mesh", meta = (EditCondition = "bOverridePawnTransform", EditConditionHides))
	FVector OverideMeshScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TSubclassOf<UAnimInstance> OverrideAnimationClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Camera")
	bool bOverrideCameraSetup = false;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (EditCondition = "bOverrideCameraSetup", EditConditionHides))
	float OverrideCameraTargetArmLength = 0;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (EditCondition = "bOverrideCameraSetup", EditConditionHides))
	FVector OverrideCameraSocketOffset = FVector::ZeroVector;
};

UCLASS()
class MYWHOOSHTASK_API AMW_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	FCharacterPawnsData* GetRandomPawnData();
	FCharacterPawnsData* GetPawnDataByTag(const FGameplayTag& PawnTag);
	FCharacterPawnsData* GetCurrentPawnData();

	void SetPawnTag(FGameplayTag InPawnTag);
	FGameplayTag GetPawnTag() { return PawnTag; }

	UFUNCTION()
	void OnRep_PawnTag();

private:
	bool LoadPawnsData();

	/** Get datatable dynamically in case using a gamemode class instead of bp */
	bool TryGetCharactersPawnData();

public:
	UPROPERTY(EditAnywhere, Category = "Character Pawns")
	UDataTable* CharactersPawnsDataTable;

	UPROPERTY(ReplicatedUsing = OnRep_PawnTag)
	FGameplayTag PawnTag;
private:
	TMap<FGameplayTag, FCharacterPawnsData*> CharacterPawns;

};
