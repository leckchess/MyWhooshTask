// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/InGameMenu.h"
#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	if (Super::Initialize() == false) { return false; }

	if (Button_Quit)
	{
		Button_Quit->OnClicked.AddDynamic(this, &UInGameMenu::OnQuitButtonCLicked);
	}

	if (Button_Cancel)
	{
		Button_Cancel->OnClicked.AddDynamic(this, &UInGameMenu::OnCancelMenuButtonCLicked);
	}

	return true;
}

void UInGameMenu::OnQuitButtonCLicked()
{
	if (MenuInterface == nullptr) { return; }

	MenuInterface->Quit();
}

void UInGameMenu::OnCancelMenuButtonCLicked()
{
	Destroy();
}
