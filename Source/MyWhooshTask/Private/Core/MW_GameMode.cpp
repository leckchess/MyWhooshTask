// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/MW_GameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Core/MW_GameInstance.h"

PRAGMA_DISABLE_OPTIMIZATION_ACTUAL
AMW_GameMode::AMW_GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Base/BP_BaseCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AMW_GameMode::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{

	if (NewPlayer->IsLocalController() == false)
	{
		UMW_GameInstance* GameInstance = Cast<UMW_GameInstance>(GetGameInstance());
		FGameplayTag ChachedRandPawnDataTag = GameInstance ? GameInstance->GetCachedRandomPawnDataTag() : FGameplayTag();

		FCharacterPawnsData* RandomPawnData = ChachedRandPawnDataTag.IsValid() ? GameInstance->CharacterPawns[ChachedRandPawnDataTag] : GetRandomPawnData();
		if (RandomPawnData && RandomPawnData->Pawn)
		{
			DefaultPawnClass = RandomPawnData->Pawn;
			GameInstance->SetCachedRandomPawnDataTag(RandomPawnData->PawnTag);
		}
	}
	else
	{
		UE_LOG(LogGameMode, Verbose, TEXT("FinishRestartPlayer: Can't assign random pawn to Server"));
	}
	Super::RestartPlayerAtPlayerStart(NewPlayer, StartSpot);
}

bool AMW_GameMode::LoadPawnsData()
{
	if (CharactersPawnsDataTable == nullptr && TryGetCharactersPawnData() == false) { return false; }
	if (UMW_GameInstance* GameInstance = Cast<UMW_GameInstance>(GetGameInstance()))
	{
		return GameInstance->LoadPawnsData(CharactersPawnsDataTable);
	}

	return false;
}

FCharacterPawnsData* AMW_GameMode::GetRandomPawnData()
{
	UMW_GameInstance* GameInstance = Cast<UMW_GameInstance>(GetGameInstance());

	if (GameInstance == nullptr || GameInstance->CharacterPawns.Num() == 0 && LoadPawnsData() == false) { return nullptr; }

	TArray<FGameplayTag> PawnsTags;
	GameInstance->CharacterPawns.GetKeys(PawnsTags);

	return GameInstance->CharacterPawns[PawnsTags[FMath::RandRange(0, PawnsTags.Num() - 1)]];
}

bool AMW_GameMode::TryGetCharactersPawnData()
{
	CharactersPawnsDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("/Game/Blueprints/DataTables/DT_Characters.DT_Characters")));
	return CharactersPawnsDataTable != nullptr;
}

void AMW_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	PlayersNumber++;

	UE_LOG(LogTemp, Error, TEXT("Player #%d has been logged in "), PlayersNumber);

	UMW_GameInstance* GameInstance = Cast<UMW_GameInstance>(GetGameInstance());
	if (!ensure(GameInstance != nullptr)) return;

	if (PlayersNumber == GameInstance->GetMaxNumberOfPlayers())
	{
		UWorld* World = GetWorld();
		if (!ensure(World != nullptr)) return;

		World->GetTimerManager().SetTimer(StartGame_Handle, this, &AMW_GameMode::StartGame, 5, false);
	}
}

void AMW_GameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	PlayersNumber--;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	if (World->GetTimerManager().IsTimerActive(StartGame_Handle))
	{
		World->GetTimerManager().ClearTimer(StartGame_Handle);
	}

	UMW_GameInstance* GameInstance = Cast<UMW_GameInstance>(GetGameInstance());

	if (GameInstance)
	{
		if (Exiting->IsLocalController())
		{
			GameInstance->DisconnectAllPlayers();
		}

		GameInstance->OnPlayerLogout(Exiting);
	}
}

void AMW_GameMode::StartGame()
{
	UMW_GameInstance* GameInstance = Cast<UMW_GameInstance>(GetGameInstance());
	if (!ensure(GameInstance != nullptr)) return;
	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Maps/Game?listen");
}

PRAGMA_ENABLE_OPTIMIZATION_ACTUAL