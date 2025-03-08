// Copyright Huda Rasmey. All Rights Reserved.


#include "Character/Human/CustomizableCharacter.h"
#include "Core/MW_GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/SpringArmComponent.h"
#include "Core/MW_PlayerState.h"
#include "Core/MW_GameInstance.h"

void ACustomizableCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CachedGameState = GetWorld()->GetGameState<AMW_GameStateBase>();
	}
}

void ACustomizableCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AMW_PlayerState* MW_PlayerState = Cast<AMW_PlayerState>(GetPlayerState()))
	{
		PlayerId = MW_PlayerState->PersistentPlayerID;
		PlayerTag = MW_PlayerState->PlayerPawnTag;
	}
}

void ACustomizableCharacter::OnRep_GameState()
{
	if (IsLocallyControlled() == false)
	{
		return;
	}

	if (CachedGameState)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("GameState Replicated!"));

		FCharacterPawnsData* PawnData = nullptr;
		if (PlayerTag.IsValid())
		{
			PawnData = CachedGameState->GetPawnDataByTag(PlayerTag);
		}
		else
		{
			PawnData = CachedGameState->GetPawnDataByNetworkId(PlayerId);
		}

		if (PawnData)
		{
			MaterialParameterCollection = GetWorld()->GetParameterCollectionInstance(PawnData->MaterialParameterCollection);
		}

		if (UMW_GameInstance* MW_GameInstance = GetGameInstance<UMW_GameInstance>())
		{
			MW_GameInstance->OnCustomizeColorButtonCLickedEvent.AddDynamic(this, &ACustomizableCharacter::ApplyColorCustomization);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("GameState is still NULL")));
	}
}

void ACustomizableCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACustomizableCharacter, CachedGameState);
	DOREPLIFETIME(ACustomizableCharacter, PlayerId);
	DOREPLIFETIME(ACustomizableCharacter, PlayerTag);
}