// Copyright Huda Rasmey. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "ColoringButton.generated.h"

/**
 *
 */
UCLASS()
class MYWHOOSHTASK_API UColoringButton : public UButton
{
	GENERATED_BODY()

public:
	FLinearColor GetButtonColor() { return ButtonColor; }
public:
	UPROPERTY(EditAnywhere)
	FLinearColor ButtonColor;

};
