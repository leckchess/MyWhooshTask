// Copyright Huda Rasmey. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "Net/UnrealNetwork.h"
#include "VehicleCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UChaosWheeledVehicleMovementComponent;
class UInputAction;
class UEnhancedInputLocalPlayerSubsystem;
struct FInputActionValue;

UCLASS()
class MYWHOOSHTASK_API AVehicleCharacter : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	AVehicleCharacter();

protected:
	/** map character input to actions */
	virtual void MapInput(APlayerController* PlayerController);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetThrottle(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetSteering(float Value);

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

	/** Look Around Action */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;

	/** Look Around Action */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;
};
