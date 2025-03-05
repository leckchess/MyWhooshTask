// Copyright Huda Rasmey. All Rights Reserved


#include "MenuSystem/ServerRow.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UServerRow::Setup(FServerData ServerData, UMainMenu* InMainMenu, uint32 InItemIndex)
{
	if (Text_ServerName)
	{
		Text_ServerName->SetText(FText::FromString(ServerData.Name));
	}

	if (Text_PlayersNumber)
	{
		FString PlayersNumber = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
		Text_PlayersNumber->SetText(FText::FromString(PlayersNumber));
	}

	if (Text_Host)
	{
		Text_Host->SetText(FText::FromString(ServerData.HostUserName));
	}

	MainMenu = InMainMenu;

	ItemIndex = InItemIndex;

	if (Button_Item)
	{
		Button_Item->OnClicked.AddDynamic(this, &UServerRow::OnItemButtonCLicked);
	}
}

void UServerRow::OnItemButtonCLicked()
{
	if (MainMenu == nullptr) { return; }
	MainMenu->SelectIndex(ItemIndex);
}
