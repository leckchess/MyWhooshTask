// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/MW_GameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Core/MW_GameInstance.h"
#include "Core/MW_GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Character/Human/BaseCharacter.h"

//PRAGMA_DISABLE_OPTIMIZATION_ACTUAL
AMW_GameMode::AMW_GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Characters/Base/BP_BaseCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	GameStateClass = AMW_GameStateBase::StaticClass();
}

void AMW_GameMode::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{
	if (NewPlayer->IsLocalController() == false)
	{
		if (AMW_GameStateBase* MWGameState = GetGameState<AMW_GameStateBase>())
		{
			UMW_GameInstance* GameInstance = Cast<UMW_GameInstance>(GetGameInstance());
			const FUniqueNetIdRepl& NewPlayerStateUniqueId = NewPlayer->PlayerState->GetUniqueId();
			uint32 NumericID = GetTypeHash(*NewPlayerStateUniqueId);

			FGameplayTag ChachedRandPawnDataTag = GameInstance && GameInstance->MappedRandomPawnes.Contains(NumericID) ? GameInstance->MappedRandomPawnes[NumericID] : FGameplayTag();

			FCharacterPawnsData* RandomPawnData = ChachedRandPawnDataTag.IsValid() ? MWGameState->GetPawnDataByTag(ChachedRandPawnDataTag) : MWGameState->GetRandomPawnData();
			if (RandomPawnData && RandomPawnData->Pawn)
			{
				DefaultPawnClass = RandomPawnData->Pawn;
				GameInstance->MappedRandomPawnes.Add(NumericID, RandomPawnData->PawnTag);
			}
		}
	}
	else
	{
		UE_LOG(LogGameMode, Verbose, TEXT("FinishRestartPlayer: Can't assign random pawn to Server"));
	}

	Super::RestartPlayerAtPlayerStart(NewPlayer, StartSpot);
}

void AMW_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// get player unique id
	const FUniqueNetIdRepl& NewPlayerStateUniqueId = NewPlayer->PlayerState->GetUniqueId();
	if (NewPlayerStateUniqueId.IsValid() && NewPlayerStateUniqueId.IsV1())
	{
		if (AMW_GameStateBase* MWGameState = GetGameState<AMW_GameStateBase>())
		{
			if (UMW_GameInstance* GameInstance = Cast<UMW_GameInstance>(GetGameInstance()))
			{
				uint32 NumericID = GetTypeHash(*NewPlayerStateUniqueId);

				if (GameInstance->MappedRandomPawnes.Contains(NumericID))
				{
					MWGameState->AssignPawnData(NumericID, GameInstance->MappedRandomPawnes[NumericID]);

					if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(NewPlayer->GetPawn()))
					{
						BaseCharacter->ActorID = NumericID;
						BaseCharacter->PawnTag = GameInstance->MappedRandomPawnes[NumericID];
					}
				}
			}
		}
	}

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

//PRAGMA_ENABLE_OPTIMIZATION_ACTUAL