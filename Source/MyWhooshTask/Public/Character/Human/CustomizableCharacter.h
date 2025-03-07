// Copyright Huda Rasmey. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/Human/BaseCharacter.h"
#include "../ICusomizableInterface.h"
#include "CustomizableCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MYWHOOSHTASK_API ACustomizableCharacter : public ABaseCharacter, public IICusomizableInterface
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	/** IICusomizableInterface */
	void ApplyCustomization(FCharacterPawnsData* CustomizationData) override;

private:
	void TryApplyCustomization();
};
