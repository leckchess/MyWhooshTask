// Copyright Huda Rasmey. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GameplayTagContainer.h"
#include "MW_GameInstance.generated.h"

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
	TSubclassOf<UStaticMesh> OverrideMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	bool bOverridePawnTransform = false;

	UPROPERTY(EditAnywhere, Category = "Mesh", meta = (ToolTip = "Relative", EditCondition = "bOverridePawnTransform", EditConditionHides))
	FVector OverrideMeshLocation;

	UPROPERTY(EditAnywhere, Category = "Mesh", meta = (ToolTip = "Relative", EditCondition = "bOverridePawnTransform", EditConditionHides))
	FVector OverrideMeshRotation;

	UPROPERTY(EditAnywhere, Category = "Mesh", meta = (EditCondition = "bOverridePawnTransform", EditConditionHides))
	FVector OverideMeshScale;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TSubclassOf<UAnimInstance> OverrideAnimationClass;

	UPROPERTY(EditAnywhere, Category = "Camera")
	bool bOverrideCameraSetup = false;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (EditCondition = "bOverrideCameraSetup", EditConditionHides))
	float OverrideCameraTargetArmLength;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (EditCondition = "bOverrideCameraSetup", EditConditionHides))
	FVector OverrideCameraSocketOffset;
};

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

	void SetCachedDefaultPawnClass(TSubclassOf<APawn> InCachedDefaultPawnClass) { CachedDefaultPawnClass = InCachedDefaultPawnClass; }
	void SetCachedRandomPawnDataTag(FGameplayTag InCachedRandomPawnDataTag) { CachedRandomPawnDataTag = InCachedRandomPawnDataTag; }

	TSubclassOf<APawn> GetCachedDefaultPawnClass() { return CachedDefaultPawnClass; }
	FGameplayTag GetCachedRandomPawnDataTag() { return CachedRandomPawnDataTag; }

	bool LoadPawnsData(UDataTable* CharactersPawnsDataTable);
private:
	void CreateSession();

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful, uint32 Index);
	void OnFindFriendSessionComplete(bool bWasSuccessful);

	/** on server crashes */
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type NetworkFailureType, const FString& Message);

public:
	TMap<FGameplayTag, FCharacterPawnsData*> CharacterPawns;

private:
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;
	TSubclassOf<class UUserWidget> InGameMenuWidgetClass;

	class UMainMenu* MainMenuWidget;

	IOnlineSessionPtr SessionInterface = nullptr;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	FString DesiredServerName;
	FString DesiredHostName;

	TSubclassOf<APawn> CachedDefaultPawnClass;
	FGameplayTag CachedRandomPawnDataTag;

};
