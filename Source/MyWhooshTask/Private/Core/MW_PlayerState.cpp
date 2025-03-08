// Copyright Huda Rasmey. All Rights Reserved.


#include "Core/MW_PlayerState.h"
#include "Net/UnrealNetwork.h"

AMW_PlayerState::AMW_PlayerState()
{
	//bPersistAcrossLevelTransition = true;
}

void AMW_PlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void AMW_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMW_PlayerState, PersistentPlayerID);
	DOREPLIFETIME(AMW_PlayerState, PlayerPawnTag);
}
