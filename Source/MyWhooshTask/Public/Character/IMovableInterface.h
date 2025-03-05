// Copyright Huda Rasmey. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IMovableInterface.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class UEnhancedInputLocalPlayerSubsystem;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIMovableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class MYWHOOSHTASK_API IIMovableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** assign context mapping */
	virtual void SetupPlayerInput(APlayerController* PlayerController);

	/** map character input to actions */
	virtual void MapInput(APlayerController* PlayerController) {};

	/** handle movement */
	virtual void Move(const FInputActionValue& Value) {};
	virtual void Look(const FInputActionValue& Value) {};

	/** Get default mapping context */
	virtual UInputMappingContext* GetDefaultMappingContext() const { return nullptr; }
};
