// Copyright Huda Rasmey. All Rights Reserved.

#include "Character/VehicleCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "ChaosVehicleMovementComponent.h"

AVehicleCharacter::AVehicleCharacter()
{
	if (GetMesh())
	{
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName(FName("Vehicle"));
	}

	// Create Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 650.0;
	CameraBoom->SocketOffset = FVector(0, 0, 150);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bReplicates = true;
}

void AVehicleCharacter::MapInput(APlayerController* PlayerController)
{
	if (PlayerController == nullptr) { return; }

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVehicleCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVehicleCharacter::Look);
	}
}

bool AVehicleCharacter::Server_SetThrottle_Validate(float Value)
{
	return true;
}

void AVehicleCharacter::Server_SetThrottle_Implementation(float Value)
{
	if (GetVehicleMovement() == nullptr) { return; }

	GetVehicleMovement()->SetThrottleInput(Value);
}

bool AVehicleCharacter::Server_SetSteering_Validate(float Value)
{
	return true;
}

void AVehicleCharacter::Server_SetSteering_Implementation(float Value)
{
	if (GetVehicleMovement() == nullptr) { return; }

	GetVehicleMovement()->SetSteeringInput(Value);
}

void AVehicleCharacter::Move(const FInputActionValue& Value)
{
	if (GetVehicleMovement() == nullptr) { return; }

	float MovementValue = Value.Get<FVector2D>().Y;

	if (HasAuthority())
	{
		GetVehicleMovement()->SetThrottleInput(MovementValue);
	}
	else
	{
		Server_SetThrottle(MovementValue);
	}
}

void AVehicleCharacter::Look(const FInputActionValue& Value)
{
	if (GetVehicleMovement() == nullptr) { return; }

	float LookValue = Value.Get<FVector2D>().X;

	if (HasAuthority())
	{
		GetVehicleMovement()->SetSteeringInput(LookValue);
	}
	else
	{
		Server_SetSteering(LookValue);
	}
}
