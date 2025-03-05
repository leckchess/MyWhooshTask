// Copyright Huda Rasmey. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidget.generated.h"

/**
 *
 */
UCLASS()
class MYWHOOSHTASK_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface* InMenuInterface) { MenuInterface = InMenuInterface; }
	void Setup();
	void Destroy();

protected:
	IMenuInterface* MenuInterface;
};
