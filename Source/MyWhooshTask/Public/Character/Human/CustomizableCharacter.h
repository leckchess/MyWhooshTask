// Copyright Huda Rasmey. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/Human/BaseCharacter.h"
#include "../ICusomizableInterface.h"
#include "CustomizableCharacter.generated.h"

class AMW_GameStateBase;

UCLASS()
class MYWHOOSHTASK_API ACustomizableCharacter : public ABaseCharacter, public IICusomizableInterface
{
	GENERATED_BODY()

public:
	/** IICusomizableInterface */
	class UMaterialParameterCollection* GetCustomizationMaterialCollection() override {return MaterialParameterCollection;}
	APawn* GetOwnerPawn() override { return this; }

	UFUNCTION()
	void OnRep_GameState();

	UFUNCTION()
	void OnRep_CustomizationColor();

	UFUNCTION(Server,Reliable)
	void Server_ApplyColorCustomization(FLinearColor NewColor, FGameplayTag PawnTag);	

	UFUNCTION()
	void OnChangeColorCall(FLinearColor InColor);

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

public:
	UPROPERTY(ReplicatedUsing = OnRep_GameState)
	AMW_GameStateBase* CachedGameState;

	UPROPERTY(Replicated)
	uint32 PlayerId;

	UPROPERTY(Replicated)
	FGameplayTag PlayerTag;

	UPROPERTY(ReplicatedUsing = OnRep_CustomizationColor)
	FLinearColor CustomizationColor;

private:
	UPROPERTY()
	UMaterialParameterCollection* MaterialParameterCollection;
};
