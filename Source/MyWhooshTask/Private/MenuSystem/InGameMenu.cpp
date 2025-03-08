// Copyright Huda Rasmey. All Rights Reserved


#include "MenuSystem/InGameMenu.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "MenuSystem/ColoringButton.h"
#include "Character/ICusomizableInterface.h"

bool UInGameMenu::Initialize()
{
	if (Super::Initialize() == false) { return false; }

	if (Overlay_Pause)
	{
		Overlay_Pause->SetVisibility(ESlateVisibility::Hidden);
	}

	if (Overlay_Customization)
	{
		Overlay_Customization->SetVisibility(ESlateVisibility::Hidden);
	}

	if (Button_Quit)
	{
		Button_Quit->OnClicked.AddDynamic(this, &UInGameMenu::OnQuitButtonCLicked);
	}

	if (Button_Cancel_Pause)
	{
		Button_Cancel_Pause->OnClicked.AddDynamic(this, &UInGameMenu::OnCancelButtonCLicked_PauseMenu);
	}

	if (Button_Cancel_Customization)
	{
		Button_Cancel_Customization->OnClicked.AddDynamic(this, &UInGameMenu::OnCancelButtonCLicked_CustomizationMenu);
	}

	if (Button_Blue)
	{
		Button_Blue->OnClicked.AddDynamic(this, &UInGameMenu::OnColorButtonCLicked_Blue);
	}

	if (Button_Green)
	{
		Button_Green->OnClicked.AddDynamic(this, &UInGameMenu::OnColorButtonCLicked_Green);
	}

	if (Button_Red)
	{
		Button_Red->OnClicked.AddDynamic(this, &UInGameMenu::OnColorButtonCLicked_Red);
	}

	if (Button_White)
	{
		Button_White->OnClicked.AddDynamic(this, &UInGameMenu::OnColorButtonCLicked_White);
	}

	UWorld* World = GetWorld();
	if (World == nullptr) { return true; }

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) { return true; }

	if (PlayerController->GetPawn())
	{
		if (PlayerController->GetPawn()->Implements<UICusomizableInterface>() == false)
		{
			TextBlock_Info->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	return true;
}

void UInGameMenu::Setup()
{
	AddToViewport();
}

void UInGameMenu::OpenPauseScreen()
{
	if (Overlay_Pause == nullptr) { return; }

	Overlay_Pause->SetVisibility(ESlateVisibility::Visible);
	FocusUI();
}

void UInGameMenu::OpenCustomizationScreen()
{
	if (Overlay_Customization == nullptr) { return; }

	UWorld* World = GetWorld();
	if (World == nullptr) { return; }

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) { return; }

	if (PlayerController->GetPawn())
	{
		if (PlayerController->GetPawn()->Implements<UICusomizableInterface>() == false)
		{
			return;
		}
	}

	Overlay_Customization->SetVisibility(ESlateVisibility::Visible);
	FocusUI();
}

void UInGameMenu::OnQuitButtonCLicked()
{
	if (MenuInterface == nullptr) { return; }

	MenuInterface->Quit();
}

void UInGameMenu::OnCancelButtonCLicked_PauseMenu()
{
	if (Overlay_Pause == nullptr) { return; }

	Overlay_Pause->SetVisibility(ESlateVisibility::Hidden);
	ClearFocus();
}

void UInGameMenu::OnCancelButtonCLicked_CustomizationMenu()
{
	if (Overlay_Customization == nullptr) { return; }

	Overlay_Customization->SetVisibility(ESlateVisibility::Hidden);
	ClearFocus();
}

void UInGameMenu::OnColorButtonCLicked_Blue()
{
	if (MenuInterface == nullptr) { return; }

	MenuInterface->OnCustomizeColorButtonCLicked(Button_Blue->GetButtonColor());
}

void UInGameMenu::OnColorButtonCLicked_Red()
{
	if (MenuInterface == nullptr) { return; }

	MenuInterface->OnCustomizeColorButtonCLicked(Button_Red->GetButtonColor());
}

void UInGameMenu::OnColorButtonCLicked_Green()
{
	if (MenuInterface == nullptr) { return; }

	MenuInterface->OnCustomizeColorButtonCLicked(Button_Green->GetButtonColor());
}

void UInGameMenu::OnColorButtonCLicked_White()
{
	if (MenuInterface == nullptr) { return; }

	MenuInterface->OnCustomizeColorButtonCLicked(Button_White->GetButtonColor());
}