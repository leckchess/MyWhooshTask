// Copyright Huda Rasmey. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../IMovableInterface.h"
#include "HumanCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UControllableComponent;
class UInputAction;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
struct FInputActionValue;

UCLASS()
class MYWHOOSHTASK_API AHumanCharacter :public ACharacter, public IIMovableInterface
{
	GENERATED_BODY()

public:
	AHumanCharacter();

protected:
	/** called when the controller change (possess/ unpossess) */
	virtual void NotifyControllerChanged() override;

	UInputMappingContext* GetDefaultMappingContext() const override;

	/** map character input to actions */
	virtual void MapInput(APlayerController* PlayerController) override;

	virtual void Move(const FInputActionValue& Value) override;
	virtual void Look(const FInputActionValue& Value) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Collision")
	FVector2D CapsuleSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pawn Movement")
	float MaxMovementSpeed;

	/** Look Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	/** Move Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

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
};
