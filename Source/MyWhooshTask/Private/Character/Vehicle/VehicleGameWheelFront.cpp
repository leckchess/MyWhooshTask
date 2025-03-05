// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/Vehicle/VehicleGameWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UVehicleGameWheelFront::UVehicleGameWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}