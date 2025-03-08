// Copyright Huda Rasmey. All Rights Reserved.

#include "Character/Vehicle/VehicleCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Net/UnrealNetwork.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Core/MW_GameStateBase.h"
#include "Core/MW_PlayerState.h"
#include "Core/MW_GameInstance.h"

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

	if (HasAuthority())
	{
		CachedGameState = GetWorld()->GetGameState<AMW_GameStateBase>();
		OnRep_GameState();
	}
}

void AVehicleCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AMW_PlayerState* MW_PlayerState = Cast<AMW_PlayerState>(GetPlayerState()))
	{
		PlayerId = MW_PlayerState->PersistentPlayerID;
		PlayerTag = MW_PlayerState->PlayerPawnTag;
	}
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

void AVehicleCharacter::OnRep_GameState()
{
	if (CachedGameState)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("GameState Replicated!"));

		FCharacterPawnsData* PawnData = nullptr;
		if (PlayerTag.IsValid())
		{
			PawnData = CachedGameState->GetPawnDataByTag(PlayerTag);
		}
		else
		{
			PawnData = CachedGameState->GetPawnDataByNetworkId(PlayerId);
		}

		if (PawnData)
		{
			MaterialParameterCollection = PawnData->MaterialParameterCollection;
		}

		if (UMW_GameInstance* MW_GameInstance = GetGameInstance<UMW_GameInstance>())
		{
			MW_GameInstance->OnCustomizeColorButtonCLickedEvent.AddDynamic(this, &AVehicleCharacter::OnChangeColorCall);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("GameState is still NULL")));
	}
}

void AVehicleCharacter::OnChangeColorCall(FLinearColor InColor)
{
	if (IsLocallyControlled() == false) { return; }

	Server_ApplyColorCustomization(InColor, PlayerTag);
}

void AVehicleCharacter::OnRep_CustomizationColor()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("ApplyColorCustomization OnRep"));

	ApplyColorCustomization(CustomizationColor);
}

void AVehicleCharacter::Server_ApplyColorCustomization_Implementation(FLinearColor NewColor, FGameplayTag InPawnTag)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("ApplyColorCustomization Server"));

	CustomizationColor = NewColor;
	if (CachedGameState)
	{
		MaterialParameterCollection = CachedGameState->GetPawnDataByTag(InPawnTag)->MaterialParameterCollection;
		ApplyColorCustomization(CustomizationColor);
	}
}

UInputMappingContext* AVehicleCharacter::GetDefaultMappingContext() const
{
	return DefaultMappingContext;
}

void AVehicleCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVehicleCharacter, CachedGameState);
	DOREPLIFETIME(AVehicleCharacter, PlayerId);
	DOREPLIFETIME(AVehicleCharacter, PlayerTag);
	DOREPLIFETIME(AVehicleCharacter, CustomizationColor);
}
