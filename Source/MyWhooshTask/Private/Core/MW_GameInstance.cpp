// Copyright Huda Rasmey. All Rights Reserved


#include "Core/MW_GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"
#include "OnlineSubsystem.h"
#include "Misc/Paths.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/OnlineReplStructs.h"
#include "Engine/DataTable.h"

//PRAGMA_DISABLE_OPTIMIZATION_ACTUAL

const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");
const static FName HOST_NAME_SETTINGS_KEY = TEXT("HostName");
const static int32 CONNECTION_NUMBER = 3;

UMW_GameInstance::UMW_GameInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	FString MainMenuWidgetBP_FilePath = FPaths::ProjectDir() + "Content/MenuSystem/Blueprints/WBP_MainMenu.uasset";
	if (FPaths::FileExists(MainMenuWidgetBP_FilePath))
	{
		static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidgetBP(TEXT("/Game/MenuSystem/Blueprints/WBP_MainMenu"));

		if (!ensure(MainMenuWidgetBP.Class != nullptr)) { return; }

		MainMenuWidgetClass = MainMenuWidgetBP.Class;
	}

	FString InGameMenuWidgetBP_FilePath = FPaths::ProjectDir() + "Content/MenuSystem/Blueprints/WBP_InGameMenu.uasset";
	if (FPaths::FileExists(InGameMenuWidgetBP_FilePath))
	{
		static ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuWidgetBP(TEXT("/Game/MenuSystem/Blueprints/WBP_InGameMenu"));

		if (!ensure(InGameMenuWidgetBP.Class != nullptr)) { return; }

		InGameMenuWidgetClass = InGameMenuWidgetBP.Class;
	}
}

void UMW_GameInstance::Init()
{
	if (MainMenuWidgetClass.Get())
	{
		UE_LOG(LogTemp, Log, TEXT("Menu class found %s"), *MainMenuWidgetClass->GetName());
	}

	if (InGameMenuWidgetClass.Get())
	{
		UE_LOG(LogTemp, Log, TEXT("InGame Menu class found %s"), *InGameMenuWidgetClass->GetName());
	}

	if (IOnlineSubsystem* SubsystemInterface = IOnlineSubsystem::Get())
	{
		UE_LOG(LogTemp, Log, TEXT("Online subsystem is found %s"), *SubsystemInterface->GetSubsystemName().ToString());
		SessionInterface = SubsystemInterface->GetSessionInterface();
		if (SessionInterface)
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMW_GameInstance::OnCreateSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMW_GameInstance::OnJoinSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMW_GameInstance::OnFindFriendSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Online subsystem is NULL"));
	}

	if (!ensure(GEngine != nullptr)) return;
	GEngine->OnNetworkFailure().AddUObject(this, &UMW_GameInstance::OnNetworkFailure);
}

void UMW_GameInstance::LoadMenu()
{
	if (!ensure(MainMenuWidgetClass.Get() != nullptr)) { return; }

	MainMenuWidget = Cast<UMainMenu>(CreateWidget(this, MainMenuWidgetClass));

	if (!ensure(MainMenuWidget != nullptr)) { return; }

	MainMenuWidget->SetMenuInterface(this);

	MainMenuWidget->Setup();
}

void UMW_GameInstance::LoadInGameMenu()
{
	if (!ensure(InGameMenuWidgetClass.Get() != nullptr)) { return; }

	UInGameMenu* InGameMenuWidget = Cast<UInGameMenu>(CreateWidget(this, InGameMenuWidgetClass));

	if (!ensure(InGameMenuWidget != nullptr)) { return; }

	InGameMenuWidget->SetMenuInterface(this);

	InGameMenuWidget->Setup();
}

void UMW_GameInstance::Host(const FString& ServerName, const FString& HostName)
{
	if (SessionInterface)
	{
		UE_LOG(LogTemp, Log, TEXT("Session Interface is found"));

		DesiredServerName = ServerName;
		DesiredHostName = HostName;

		if (SessionInterface->GetNamedSession(NAME_GameSession))
		{
			uint32 index = -1;
			UE_LOG(LogTemp, Warning, TEXT("Session already exists! Destroying old session..."));
			SessionInterface->DestroySession(NAME_GameSession,
				FOnDestroySessionCompleteDelegate::CreateUObject(this, &UMW_GameInstance::OnDestroySessionComplete, index));
			return;
		}

		CreateSession();
	}
}

void UMW_GameInstance::Join(uint32 index)
{
	if (SessionInterface == nullptr || SessionSearch == nullptr) { return; }

	if (MainMenuWidget)
	{
		MainMenuWidget->Destroy();
	}

	if (SessionInterface->GetNamedSession(NAME_GameSession))
	{
		UE_LOG(LogTemp, Warning, TEXT("Session already exists! Destroying old session..."));
		SessionInterface->DestroySession(NAME_GameSession,
			FOnDestroySessionCompleteDelegate::CreateUObject(this, &UMW_GameInstance::OnDestroySessionComplete, index));
		return;
	}

	SessionInterface->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[index]);
}

void UMW_GameInstance::Quit()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (PlayerController)
	{
		PlayerController->ClientTravel("/Game/MenuSystem/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
		OnPlayerLogout(PlayerController);
	}

	if (PlayerController == nullptr || PlayerController->IsLocalController())
	{
		DisconnectAllPlayers();
	}
}

void UMW_GameInstance::RequestServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch)
	{
		SessionSearch->bIsLanQuery = true;

		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_KEYWORDS, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UMW_GameInstance::OnPlayerLogout(AController* LogoutController)
{
	if (SessionInterface.IsValid() == false) { return; }

	if (SessionInterface.IsValid() && LogoutController && LogoutController->PlayerState)
	{
		if (APlayerState* PlayerState = LogoutController->GetPlayerState<APlayerState>())
		{
			SessionInterface->UnregisterPlayer(NAME_GameSession, *PlayerState->GetUniqueId());
			UE_LOG(LogTemp, Log, TEXT("Player Loggedout"));
		}
	}
}

void UMW_GameInstance::StartSession()
{
	if (SessionInterface == nullptr) { return; }

	SessionInterface->StartSession(NAME_GameSession);
}

int32 UMW_GameInstance::GetMaxNumberOfPlayers()
{
	return CONNECTION_NUMBER;
}

void UMW_GameInstance::DisconnectAllPlayers()
{
	if (GetWorld() == nullptr) { return; }

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController && !PlayerController->IsLocalController())
		{
			PlayerController->ClientTravel("/Game/MenuSystem/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
			OnPlayerLogout(PlayerController);
		}
	}
}

void UMW_GameInstance::Shutdown()
{
	DisconnectAllPlayers();
}

void UMW_GameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create session %s"), *SessionName.ToString());
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Session %s is creates successfully"), *SessionName.ToString());

	if (MainMenuWidget)
	{
		MainMenuWidget->Destroy();
	}

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	World->ServerTravel("/Game/MenuSystem/Maps/Lobby?listen");

	if (!ensure(GEngine != nullptr)) return;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Hosting..... "));
}

void UMW_GameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to connect to session %s"), *SessionName.ToString());
		return;
	}

	if (SessionInterface == nullptr) { return; }

	UE_LOG(LogTemp, Log, TEXT("Session %s is connected successfully"), *SessionName.ToString());

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Log, TEXT("couldn't find connect string for session %s"), *SessionName.ToString());
		return;
	}

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);

	if (!ensure(GEngine != nullptr)) return;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Joining %s"), *Address));
}

void UMW_GameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful, uint32 Index)
{
	if (bWasSuccessful == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to destroy session %s"), *SessionName.ToString());
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Session %s is destroyed successfully"), *SessionName.ToString());

	if (Index == -1)
	{
		CreateSession();
	}
	else
	{
		Join(Index);
	}
}

void UMW_GameInstance::OnFindFriendSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find sessions"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Sessions Found"));

	if (SessionSearch && MainMenuWidget)
	{
		TArray<FServerData> ResultList;

		if (SessionSearch)
		{
			for (const FOnlineSessionSearchResult& Session : SessionSearch->SearchResults)
			{
				FServerData Data;
				Data.Name = Session.GetSessionIdStr();
				Data.MaxPlayers = Session.Session.SessionSettings.NumPublicConnections;
				Data.CurrentPlayers = Data.MaxPlayers - Session.Session.NumOpenPublicConnections;
				Data.HostUserName = Session.Session.OwningUserName;
				FString ServerName;
				if (Session.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
				{
					Data.Name = ServerName;
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT(" not Fould"));
				}

				FString HostName;
				if (Session.Session.SessionSettings.Get(HOST_NAME_SETTINGS_KEY, HostName))
				{
					Data.HostUserName = HostName;
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT(" not Fould"));
				}

				ResultList.Add(Data);
			}
		}

		MainMenuWidget->SetServersList(ResultList);
	}
}

void UMW_GameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type NetworkFailureType, const FString& Message)
{
	Quit();

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController == nullptr)
	{
		DisconnectAllPlayers();
	}
}

void UMW_GameInstance::CreateSession()
{
	if (SessionInterface)
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = false;
		SessionSettings.NumPublicConnections = CONNECTION_NUMBER;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bUseLobbiesIfAvailable = true;

		if (DesiredServerName.IsEmpty() == false)
		{
			SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		}

		if (DesiredHostName.IsEmpty() == false)
		{
			SessionSettings.Set(HOST_NAME_SETTINGS_KEY, DesiredHostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		}

		if (IOnlineSubsystem* SubsystemInterface = IOnlineSubsystem::Get())
		{
			if (SubsystemInterface->GetSubsystemName().ToString() == "NULL")
			{
				SessionSettings.bIsLANMatch = true;
			}
		}
		SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
	}
}

//PRAGMA_ENABLE_OPTIMIZATION_ACTUAL
