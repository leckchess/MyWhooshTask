// Copyright Huda Rasmey. All Rights Reserved.


#include "Character/IMovableInterface.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Add default functionality here for any IIMovableInterface functions that are not pure virtual.

void IIMovableInterface::SetupPlayerInput(APlayerController* PlayerController)
{
	if (PlayerController == nullptr)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(GetDefaultMappingContext(), 0);
		MapInput(PlayerController);
	}
}
