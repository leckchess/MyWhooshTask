// Copyright Huda Rasmey. All Rights Reserved.


#include "Core/MW_GameStateBase.h"
#include "Net/UnrealNetwork.h"

FCharacterPawnsData* AMW_GameStateBase::GetRandomPawnData()
{
	if (CharacterPawns.Num() == 0 && LoadPawnsData() == false) { return nullptr; }

	TArray<FGameplayTag> PawnsTags;
	CharacterPawns.GetKeys(PawnsTags);

	return CharacterPawns[PawnsTags[FMath::RandRange(0, PawnsTags.Num() - 1)]];
}

FCharacterPawnsData* AMW_GameStateBase::GetPawnDataByTag(const FGameplayTag& InPawnTag)
{
	if (InPawnTag.IsValid() == false) { return nullptr; }

	if (CharacterPawns.Num() == 0 && LoadPawnsData() == false) { return nullptr; }

	return CharacterPawns[InPawnTag];
}

FCharacterPawnsData* AMW_GameStateBase::GetPawnDataByNetworkId(uint32 NetworkId)
{
	return GetPawnDataByTag(GetPawnTagByNetworkId(NetworkId));
}

FGameplayTag AMW_GameStateBase::GetPawnTagByNetworkId(uint32 NetworkId)
{
	for (const FPawnsMapping& PawnMappingData : PawnsMappingArray)
	{
		if (PawnMappingData.NetworkGUID == NetworkId)
		{
			return PawnMappingData.GameplayTag;
		}
	}

	return FGameplayTag{};
}

void AMW_GameStateBase::AssignPawnData(uint32 NetworkId, FGameplayTag pawnTag)
{
	PawnsMappingArray.Add(FPawnsMapping{ NetworkId, pawnTag });
}

void AMW_GameStateBase::OnRep_PawnsMappingArray()
{
	UE_LOG(LogTemp, Error, TEXT("Array Replication %d"), PawnsMappingArray.Num());
}

bool AMW_GameStateBase::LoadPawnsData()
{
	if (CharactersPawnsDataTable == nullptr && TryGetCharactersPawnData() == false) { return false; }

	const TMap<FName, uint8*>& AllRows = CharactersPawnsDataTable->GetRowMap();

	for (TMap<FName, uint8*>::TConstIterator RowMapIter(AllRows.CreateConstIterator()); RowMapIter; ++RowMapIter)
	{
		if (FCharacterPawnsData* CharacterPawnData = reinterpret_cast<FCharacterPawnsData*>(RowMapIter.Value()))
		{
			CharacterPawns.Add(CharacterPawnData->PawnTag, CharacterPawnData);
		}
	}

	return CharacterPawns.Num() > 0;
}

bool AMW_GameStateBase::TryGetCharactersPawnData()
{
	CharactersPawnsDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("/Game/Blueprints/DataTables/DT_Characters.DT_Characters")));
	return CharactersPawnsDataTable != nullptr;
}

void AMW_GameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMW_GameStateBase, PawnsMappingArray);
}