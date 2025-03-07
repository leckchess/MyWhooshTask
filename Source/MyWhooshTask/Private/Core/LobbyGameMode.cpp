// Copyright Huda Rasmey. All Rights Reserved

#include "Core/LobbyGameMode.h"
#include "Core/MW_GameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
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

		World->GetTimerManager().SetTimer(StartGame_Handle, this, &ALobbyGameMode::StartGame, 5, false);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
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

void ALobbyGameMode::StartGame()
{
	UMW_GameInstance* GameInstance = Cast<UMW_GameInstance>(GetGameInstance());
	if (!ensure(GameInstance != nullptr)) return;
	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Maps/Game?listen");
}
