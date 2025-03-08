// Copyright Huda Rasmey. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "../IMovableInterface.h"
#include "../ICusomizableInterface.h"
#include "Core/MW_GameStateBase.h"
#include "VehicleCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UChaosWheeledVehicleMovementComponent;
class UInputAction;
struct FInputActionValue;
class UInputMappingContext;

UCLASS()
class MYWHOOSHTASK_API AVehicleCharacter : public AWheeledVehiclePawn, public IIMovableInterface, public IICusomizableInterface
{
	GENERATED_BODY()

public:
	AVehicleCharacter();

	/** IIMovableInterface */
	UInputMappingContext* GetDefaultMappingContext() const override;
	/** map character input to actions */
	virtual void MapInput(APlayerController* PlayerController) override;
	virtual void Move(const FInputActionValue& Value) override;
	virtual void Look(const FInputActionValue& Value) override;


	/** IICusomizableInterface */
	class UMaterialParameterCollection* GetCustomizationMaterialCollection() override { return MaterialParameterCollection; };
	APawn* GetOwnerPawn() override {return this;}

	UFUNCTION()
	void OnRep_GameState();

	UFUNCTION()
	void OnRep_CustomizationColor();

	UFUNCTION(Server, Reliable)
	void Server_ApplyColorCustomization(FLinearColor NewColor, FGameplayTag PawnTag);

	UFUNCTION()
	void OnChangeColorCall(FLinearColor InColor);

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	/** called when the controller change (possess/ unpossess) */
	virtual void NotifyControllerChanged() override;

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
	/** Camera Spring Arm Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	/** MappingContext */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	/** Look Around Action */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;

	/** Look Around Action */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	/** Chaos Vehicle movement component */
	TObjectPtr<UChaosWheeledVehicleMovementComponent> ChaosVehicleMovement;

	UPROPERTY()
	UMaterialParameterCollection* MaterialParameterCollection;
};
