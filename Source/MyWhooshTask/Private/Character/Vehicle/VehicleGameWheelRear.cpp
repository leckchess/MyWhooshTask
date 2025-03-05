// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/Vehicle/VehicleGameWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UVehicleGameWheelRear::UVehicleGameWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}