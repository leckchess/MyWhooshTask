// Copyright Huda Rasmey. All Rights Reserved.


#include "Character/ICusomizableInterface.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

void IICusomizableInterface::ApplyColorCustomization(FLinearColor NewColor)
{
	APawn* OwnerPawn = GetOwnerPawn();
	if (OwnerPawn == nullptr) { return; }

	UMaterialParameterCollection* CustomizableMaterialCollection = const_cast<UMaterialParameterCollection*>(GetCustomizationMaterialCollection()->GetCollection());

	if (CustomizableMaterialCollection == nullptr) { return; }

	UWorld* World = OwnerPawn->GetWorld();
	if (World == nullptr) { return; }
	
	UKismetMaterialLibrary::SetVectorParameterValue(World, CustomizableMaterialCollection, "OverrideColor", NewColor);

}
