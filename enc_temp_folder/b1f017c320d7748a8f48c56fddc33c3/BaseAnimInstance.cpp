// Copyright Huda Rasmey. All Rights Reserved.


#include "Character/Animation/BaseAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

void UBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerPawnRef = TryGetPawnOwner();
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerPawnRef == nullptr)
	{
		return;
	}

	Speed = OwnerPawnRef->GetVelocity().Size();
	bIsFalling = OwnerPawnRef->GetMovementComponent()->IsFalling();
}
