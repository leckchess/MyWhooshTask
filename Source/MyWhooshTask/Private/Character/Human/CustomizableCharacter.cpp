// Copyright Huda Rasmey. All Rights Reserved.


#include "Character/Human/CustomizableCharacter.h"
#include "Core/MW_GameStateBase.h"
#include "GameFramework/SpringArmComponent.h"

void ACustomizableCharacter::BeginPlay()
{
	Super::BeginPlay();

	TryApplyCustomization();
}

void ACustomizableCharacter::ApplyCustomization(FCharacterPawnsData* CustomizationData)
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
		if (GetCameraBoom())
		{
			GetCameraBoom()->TargetArmLength = CustomizationData->OverrideCameraTargetArmLength;
			GetCameraBoom()->SocketOffset = CustomizationData->OverrideCameraSocketOffset;
		}
	}
}

void ACustomizableCharacter::TryApplyCustomization()
{
	if (GetLocalRole() == ROLE_Authority) { return; }

	if (AMW_GameStateBase* MW_GameState = GetWorld()->GetGameState<AMW_GameStateBase>())
	{
		if (MW_GameState->GetPawnTag().IsValid())
		{
			ApplyCustomization(MW_GameState->GetCurrentPawnData());
		}
		else
		{
			// in case replication is delayed
			GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ACustomizableCharacter::TryApplyCustomization);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ACustomizableCharacter::TryApplyCustomization);
	}
}
