// Copyright Huda Rasmey. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class MYWHOOSHTASK_API IMenuInterface
{
	GENERATED_BODY()

// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Host(const FString& ServerName, const FString& HostName) = 0;
	virtual void Join(uint32 index) = 0;
	virtual void Quit() = 0;
	virtual void RequestServerList() = 0;
	virtual void OnCustomizeColorButtonCLicked(const FLinearColor InColor) = 0;
};
