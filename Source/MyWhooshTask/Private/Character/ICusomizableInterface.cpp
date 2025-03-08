// Copyright Huda Rasmey. All Rights Reserved.


#include "Character/ICusomizableInterface.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

void IICusomizableInterface::ApplyColorCustomization(FLinearColor NewColor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("ApplyColorCustomization Interface 0"));

	APawn* OwnerPawn = GetOwnerPawn();
	if (OwnerPawn == nullptr) { return; }

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("ApplyColorCustomization Interface 1"));

	UMaterialParameterCollection* CustomizableMaterialCollection = GetCustomizationMaterialCollection();
	if (CustomizableMaterialCollection == nullptr) { return; }

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("ApplyColorCustomization Interface 2"));

	UWorld* World = OwnerPawn->GetWorld();
	if (World == nullptr) { return; }
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("ApplyColorCustomization Interface 3"));

	UKismetMaterialLibrary::SetVectorParameterValue(World, CustomizableMaterialCollection, "OverrideColor", NewColor);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("ApplyColorCustomization Interface 4"));
}
