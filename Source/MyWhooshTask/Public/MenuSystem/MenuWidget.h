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
	virtual void Setup();
	virtual void Destroy();

	void FocusUI();
	void ClearFocus();

protected:
	IMenuInterface* MenuInterface;
};
