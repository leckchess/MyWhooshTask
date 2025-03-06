// Copyright Huda Rasmey. All Rights Reserved.


#include "Character/Animation/BaseAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Character/Human/BaseCharacter.h"

void UBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerPawnRef = Cast<ABaseCharacter>(TryGetPawnOwner());
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerPawnRef == nullptr)
	{
		return;
	}

	Speed = OwnerPawnRef->GetVelocity().Size();
	Leaning = FMath::Lerp(Leaning, OwnerPawnRef->GeatLeaningValue() * 100.0f, DeltaSeconds);
	bIsFalling = OwnerPawnRef->GetMovementComponent()->IsFalling();
}
