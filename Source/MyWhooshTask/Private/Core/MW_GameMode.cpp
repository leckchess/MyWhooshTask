// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/MW_GameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"

AMW_GameMode::AMW_GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Base/BP_BaseCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		CachedDefaultPawnClass = DefaultPawnClass;
	}
}

void AMW_GameMode::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{
	if (NewPlayer->IsLocalController() == false)
	{
		FCharacterPawnsData* RandomPawnData = GetRandomPawnData();
		if (RandomPawnData)
		{
			if (RandomPawnData->Pawn)
			{
				DefaultPawnClass = RandomPawnData->Pawn;
			}
		}
	}
	else
	{
		DefaultPawnClass = CachedDefaultPawnClass;
		UE_LOG(LogGameMode, Verbose, TEXT("FinishRestartPlayer: Can't assign random pawn in Server"));
	}

	Super::RestartPlayerAtPlayerStart(NewPlayer, StartSpot);
}

bool AMW_GameMode::LoadPawnsData()
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

FCharacterPawnsData* AMW_GameMode::GetRandomPawnData()
{
	if (CharacterPawns.Num() == 0 && LoadPawnsData() == false) { return nullptr; }

	TArray<FGameplayTag> PawnsTags;
	CharacterPawns.GetKeys(PawnsTags);

	return CharacterPawns[PawnsTags[FMath::RandRange(0, PawnsTags.Num() - 1)]];
}

bool AMW_GameMode::TryGetCharactersPawnData()
{
	CharactersPawnsDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("/Game/Blueprints/DataTables/DT_Characters.DT_Characters")));
	return CharactersPawnsDataTable != nullptr;
}
