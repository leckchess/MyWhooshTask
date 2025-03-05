// Copyright Huda Rasmey. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "VehicleCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UChaosWheeledVehicleMovementComponent;
class UInputAction;
class UEnhancedInputLocalPlayerSubsystem;
struct FInputActionValue;
class UInputMappingContext;

UCLASS()
class MYWHOOSHTASK_API AVehicleCharacter : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	AVehicleCharacter();

protected:
	/** called when the controller change (possess/ unpossess) */
	virtual void NotifyControllerChanged() override;

	/** map character input to actions */
	virtual void MapInput(APlayerController* PlayerController);

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

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

	/** cached Subsystem */
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem;

	/** Chaos Vehicle movement component */
	TObjectPtr<UChaosWheeledVehicleMovementComponent> ChaosVehicleMovement;
};
