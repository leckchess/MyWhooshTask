// Copyright Huda Rasmey. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GameplayTagContainer.h"
#include "MW_GameInstance.generated.h"

UCLASS()
class MYWHOOSHTASK_API UMW_GameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

	UMW_GameInstance(const FObjectInitializer& ObjectInitializer);

public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(Exec)
	virtual void Host(const FString& ServerName, const FString& HostName) override;

	UFUNCTION(Exec)
	virtual void Join(uint32 index) override;

	UFUNCTION(Exec)
	virtual void Quit() override;

	virtual void RequestServerList() override;

	void OnPlayerLogout(AController* LogoutController);

	void StartSession();

	int32 GetMaxNumberOfPlayers();

	void DisconnectAllPlayers();

	/** server disconnected */
	void Shutdown() override;

private:
	void CreateSession();

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful, uint32 Index);
	void OnFindFriendSessionComplete(bool bWasSuccessful);

	/** on server crashes */
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type NetworkFailureType, const FString& Message);

public:
	TMap<uint64, FGameplayTag> MappedRandomPawnes;

private:
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;
	TSubclassOf<class UUserWidget> InGameMenuWidgetClass;

	class UMainMenu* MainMenuWidget;

	IOnlineSessionPtr SessionInterface = nullptr;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	FString DesiredServerName;
	FString DesiredHostName;
};
