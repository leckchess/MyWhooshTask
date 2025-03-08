// Copyright Huda Rasmey. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICusomizableInterface.generated.h"

struct FCharacterPawnsData;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UICusomizableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class MYWHOOSHTASK_API IICusomizableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ApplyCustomization() {};
	virtual class UMaterialParameterCollection* GetCustomizationMaterialCollection() { return nullptr; }
	virtual class APawn* GetOwnerPawn() { return nullptr; }

	UFUNCTION()
	virtual void ApplyColorCustomization(FLinearColor NewColor);

};
