// Copyright Huda Rasmey. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "MW_GameStateBase.generated.h"

class UDataTable;

USTRUCT(BlueprintType)
struct FPawnsMapping
{
	GENERATED_BODY()

public:
	// Unique identifier for networking (GUID)
	UPROPERTY(EditAnywhere)
	uint32 NetworkGUID;

	// Pawn Gameplay tag
	UPROPERTY(EditAnywhere)
	FGameplayTag GameplayTag;

	FPawnsMapping() {}

	FPawnsMapping(uint32 InNetworkId, FGameplayTag InPawnTag) : NetworkGUID(InNetworkId), GameplayTag(InPawnTag) {}
};

USTRUCT(BlueprintType)
struct FCharacterPawnsData : public FTableRowBase
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, Category = "Tag")
	FGameplayTag PawnTag;

	UPROPERTY(EditAnywhere, Category = "Pawn")
	TSubclassOf<APawn> Pawn;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialParameterCollection* MaterialParameterCollection;
};

UCLASS()
class MYWHOOSHTASK_API AMW_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	FCharacterPawnsData* GetRandomPawnData();
	FCharacterPawnsData* GetPawnDataByTag(const FGameplayTag& PawnTag);

	FCharacterPawnsData* GetPawnDataByNetworkId(uint32 NetworkId);
	FGameplayTag GetPawnTagByNetworkId(uint32 NetworkId);

	void AssignPawnData(uint32 NetworkId, FGameplayTag pawnTag);

	UFUNCTION()
	void OnRep_PawnsMappingArray();

private:
	bool LoadPawnsData();

	/** Get datatable dynamically in case using a class class instead of bp */
	bool TryGetCharactersPawnData();

public:
	UPROPERTY(EditAnywhere, Category = "Character Pawns")
	UDataTable* CharactersPawnsDataTable;

	UPROPERTY(ReplicatedUsing = OnRep_PawnsMappingArray)
	TArray<FPawnsMapping> PawnsMappingArray;

private:
	TMap<FGameplayTag, FCharacterPawnsData*> CharacterPawns;

};
