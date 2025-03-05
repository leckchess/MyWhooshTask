// Copyright Huda Rasmey. All Rights Reserved.

#include "Character/Vehicle/VehicleCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "ChaosWheeledVehicleMovementComponent.h"

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

	ChaosVehicleMovement = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());


	bReplicates = true;
}

void AVehicleCharacter::NotifyControllerChanged()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (Subsystem == nullptr)
		{
			Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		}

		if (Subsystem)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			MapInput(PlayerController);
		}
	}
	else
	{
		if (Subsystem)
		{
			Subsystem->RemoveMappingContext(DefaultMappingContext);
		}
	}
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

void AVehicleCharacter::Move(const FInputActionValue& Value)
{
	if (ChaosVehicleMovement == nullptr) { return; }

	float MovementValue = Value.Get<FVector2D>().Y;
	MovementValue = FMath::Clamp(MovementValue, 0, MovementValue);
	ChaosVehicleMovement->SetThrottleInput(MovementValue);
}

void AVehicleCharacter::Look(const FInputActionValue& Value)
{
	if (ChaosVehicleMovement == nullptr) { return; }

	float LookValue = Value.Get<FVector2D>().X;
	ChaosVehicleMovement->SetSteeringInput(LookValue);
}
