// Copyright Huda Rasmey. All Rights Reserved.

#include "Character/Vehicle/VehicleCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Core/MW_GameStateBase.h"

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

void AVehicleCharacter::BeginPlay()
{
	Super::BeginPlay();

	TryApplyCustomization();
}

void AVehicleCharacter::NotifyControllerChanged()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		SetupPlayerInput(PlayerController);
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
	ChaosVehicleMovement->SetThrottleInput(MovementValue);

	ChaosVehicleMovement->SetBrakeInput(MovementValue < 0 ? 1 : 0);
}

void AVehicleCharacter::Look(const FInputActionValue& Value)
{
	if (ChaosVehicleMovement == nullptr) { return; }

	float LookValue = Value.Get<FVector2D>().X;
	ChaosVehicleMovement->SetSteeringInput(LookValue);
}

void AVehicleCharacter::TryApplyCustomization()
{
	if (GetLocalRole() == ROLE_Authority) { return; }

	if (AMW_GameStateBase* MW_GameState = GetWorld()->GetGameState<AMW_GameStateBase>())
	{
		//if (MW_GameState->GetPawnTag().IsValid())
		//{
		//	ApplyCustomization(MW_GameState->GetCurrentPawnData());
		//}
		//else
		//{
		//	// in case replication is delayed
		//	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AVehicleCharacter::TryApplyCustomization);
		//}
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AVehicleCharacter::TryApplyCustomization);
	}
}

void AVehicleCharacter::ApplyCustomization(FCharacterPawnsData* CustomizationData)
{
	if (CustomizationData == nullptr) { return; }

	if (USkeletalMeshComponent* SkeletalMeshComp = GetMesh())
	{
		if (CustomizationData->OverrideMesh)
		{
			SkeletalMeshComp->SetSkeletalMesh(Cast<USkeletalMesh>(CustomizationData->OverrideMesh));
		}

		if (CustomizationData->OverrideAnimationClass)
		{
			SkeletalMeshComp->SetAnimInstanceClass(CustomizationData->OverrideAnimationClass);
			SkeletalMeshComp->InitAnim(true);
		}

		if (CustomizationData->bOverridePawnTransform)
		{
			SkeletalMeshComp->SetRelativeLocation(CustomizationData->OverrideMeshLocation);
			SkeletalMeshComp->SetRelativeRotation(FRotator::MakeFromEuler(CustomizationData->OverrideMeshRotation));
			SkeletalMeshComp->SetRelativeScale3D(CustomizationData->OverideMeshScale);
		}

		SkeletalMeshComp->RecreateRenderState_Concurrent();
	}

	if (CustomizationData->bOverrideCameraSetup)
	{
		if (CameraBoom)
		{
			CameraBoom->TargetArmLength = CustomizationData->OverrideCameraTargetArmLength;
			CameraBoom->SocketOffset = CustomizationData->OverrideCameraSocketOffset;
		}
	}
}

UInputMappingContext* AVehicleCharacter::GetDefaultMappingContext() const
{
	return DefaultMappingContext;
}
