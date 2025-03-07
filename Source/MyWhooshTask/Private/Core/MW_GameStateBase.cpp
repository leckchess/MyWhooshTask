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
	if (CharacterPawns.Num() == 0 && LoadPawnsData() == false) { return nullptr; }

	return CharacterPawns[InPawnTag];
}

FCharacterPawnsData* AMW_GameStateBase::GetCurrentPawnData()
{
	return GetPawnDataByTag(PawnTag);
}

void AMW_GameStateBase::SetPawnTag(FGameplayTag InPawnTag)
{
	if (HasAuthority())
	{
		PawnTag = InPawnTag;
	}
}

void AMW_GameStateBase::OnRep_PawnTag()
{
	if (PawnTag.IsValid())
	{
		FCharacterPawnsData* data = GetPawnDataByTag(PawnTag);
		if (data)
		{
			UE_LOG(LogTemp,Error,TEXT("Data"));
		}
	}
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
	DOREPLIFETIME(AMW_GameStateBase, PawnTag);
}