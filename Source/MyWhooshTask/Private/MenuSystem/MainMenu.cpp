// Copyright Huda Rasmey. All Rights Reserved


#include "MenuSystem/MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"
#include "Components/EditableText.h"
#include "Components/Image.h"
#include "Misc/Paths.h"
#include "MenuSystem/ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	FString ServerRowWidgetBP_FilePath = FPaths::ProjectDir() + "Content/MenuSystem/BluePrints/WBP_ServerRow.uasset";
	if (FPaths::FileExists(ServerRowWidgetBP_FilePath) == false)
	{
		return;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> ServerRowWidgetBP(TEXT("/Game/MenuSystem/BluePrints/WBP_ServerRow"));

	if (!ensure(ServerRowWidgetBP.Class != nullptr)) { return; }

	ServerRowClass = ServerRowWidgetBP.Class;
}

bool UMainMenu::Initialize()
{
	if (Super::Initialize() == false) { return false; }

	if (Button_Host)
	{
		Button_Host->OnClicked.AddDynamic(this, &UMainMenu::OnHostButtonCLicked);
	}

	if (Button_Host_Menu)
	{
		Button_Host_Menu->OnClicked.AddDynamic(this, &UMainMenu::OnHostMenuButtonCLicked);
	}

	if (Button_Join_Menu)
	{
		Button_Join_Menu->OnClicked.AddDynamic(this, &UMainMenu::OnJoinMenuButtonCLicked);
	}

	if (Button_Join)
	{
		Button_Join->OnClicked.AddDynamic(this, &UMainMenu::OnJoinButtonCLicked);
	}
	if (Button_Refresh)
	{
		Button_Refresh->OnClicked.AddDynamic(this, &UMainMenu::OnRefreshButtonCLicked);
	}

	if (Button_Back)
	{
		Button_Back->OnClicked.AddDynamic(this, &UMainMenu::OnBackButtonCLicked);
	}
	if (Button_Quit)
	{
		Button_Quit->OnClicked.AddDynamic(this, &UMainMenu::OnQuitButtonCLicked);
	}
	if (Button_Back0)
	{
		Button_Back0->OnClicked.AddDynamic(this, &UMainMenu::OnBackButtonCLicked);
	}

	return true;
}

void UMainMenu::SetServersList(TArray<FServerData> ServersList)
{
	if (ServerRowClass == nullptr || ScrollBox_Sessions == nullptr || MenuInterface == nullptr) { return; }

	ScrollBox_Sessions->ClearChildren();

	if (Image_Loading)
	{
		Image_Loading->SetVisibility(ESlateVisibility::Hidden);
	}

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	uint32 Index = 0;

	for (const FServerData& ServerData : ServersList)
	{
		UServerRow* ServerRow = CreateWidget<UServerRow>(World, ServerRowClass);
		ServerRow->Setup(ServerData, this, Index);

		if (!ensure(ServerRow != nullptr)) return;
		ScrollBox_Sessions->AddChild(ServerRow);
		Index++;
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildrenVisual();
}

void UMainMenu::OnHostMenuButtonCLicked()
{
	if (WidgetSwitcher == nullptr || Overlay_HostMenu == nullptr) { return; }

	WidgetSwitcher->SetActiveWidget(Overlay_HostMenu);
}

void UMainMenu::OnHostButtonCLicked()
{
	if (MenuInterface == nullptr || EditableText_HostName == nullptr || EditableText_ServerName == nullptr) { return; }

	MenuInterface->Host(EditableText_ServerName->GetText().ToString(), EditableText_HostName->GetText().ToString());
}

void UMainMenu::OnJoinMenuButtonCLicked()
{
	if (WidgetSwitcher == nullptr || Overlay_JoinMenu == nullptr) { return; }

	if (Image_Loading)
	{
		Image_Loading->SetVisibility(ESlateVisibility::Visible);
	}

	WidgetSwitcher->SetActiveWidget(Overlay_JoinMenu);
	MenuInterface->RequestServerList();
}

void UMainMenu::OnJoinButtonCLicked()
{
	if (MenuInterface == nullptr) { return; }

	if (SelectedIndex.IsSet() == false) { return; }

	UE_LOG(LogTemp, Error, TEXT("SelectedIndex %d"), SelectedIndex.GetValue());
	MenuInterface->Join(SelectedIndex.GetValue());
}

void UMainMenu::OnRefreshButtonCLicked()
{
	if (MenuInterface == nullptr) { return; }

	if (Image_Loading)
	{
		Image_Loading->SetVisibility(ESlateVisibility::Visible);
	}

	ScrollBox_Sessions->ClearChildren();
	MenuInterface->RequestServerList();
}

void UMainMenu::OnBackButtonCLicked()
{
	if (WidgetSwitcher == nullptr || Overlay_MainMenu == nullptr) { return; }

	WidgetSwitcher->SetActiveWidget(Overlay_MainMenu);
}

void UMainMenu::OnQuitButtonCLicked()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

#if WITH_EDITOR
	PlayerController->ConsoleCommand("quit");
#else
	FGenericPlatformMisc::RequestExit(false);
#endif
}

void UMainMenu::UpdateChildrenVisual()
{
	TArray<UWidget*> Children = ScrollBox_Sessions->GetAllChildren();
	for (UWidget* Child : Children)
	{
		if (UServerRow* SRChild = Cast<UServerRow>(Child))
		{
			SRChild->Selected = (SelectedIndex.IsSet() && SRChild->ItemIndex == SelectedIndex.GetValue());
		}
	}
}

