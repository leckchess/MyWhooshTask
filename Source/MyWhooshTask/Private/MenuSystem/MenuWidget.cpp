// Copyright Huda Rasmey. All Rights Reserved


#include "MenuSystem/MenuWidget.h"

void UMenuWidget::Setup()
{
	AddToViewport();

	FocusUI();
}

void UMenuWidget::Destroy()
{
	RemoveFromParent();

	ClearFocus();
}

void UMenuWidget::FocusUI()
{
	UWorld* World = GetWorld();
	if (World == nullptr) { return; }

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) { return; }

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(true);
}

void UMenuWidget::ClearFocus()
{
	UWorld* World = GetWorld();
	if (World == nullptr) { return; }

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) { return; }

	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(false);
}
