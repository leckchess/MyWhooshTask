// Copyright Huda Rasmey. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "../IMovableInterface.h"
#include "../ICusomizableInterface.h"
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

	virtual void BeginPlay() override;

	UInputMappingContext* GetDefaultMappingContext() const override;

	/** map character input to actions */
	virtual void MapInput(APlayerController* PlayerController) override;

	virtual void Move(const FInputActionValue& Value) override;
	virtual void Look(const FInputActionValue& Value) override;


	/** IICusomizableInterface */
	void ApplyCustomization(FCharacterPawnsData* CustomizationData) override;

protected:
	/** called when the controller change (possess/ unpossess) */
	virtual void NotifyControllerChanged() override;

private:
	void TryApplyCustomization();

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
};
