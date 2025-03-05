// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/MW_GameMode.h"
#include "UObject/ConstructorHelpers.h"

AMW_GameMode::AMW_GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_VehicleCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
