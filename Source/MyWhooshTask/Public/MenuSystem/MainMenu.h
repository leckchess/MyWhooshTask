// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;
};

/**
 *
 */
UCLASS()
class MYWHOOSHTASK_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

	UMainMenu(const FObjectInitializer& ObjectInitializer);

public:
	void SetServersList(TArray<FServerData> InServersList);

	void SelectIndex(uint32 Index);

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void OnHostMenuButtonCLicked(); 
	UFUNCTION()
	void OnHostButtonCLicked();
	UFUNCTION()
	void OnJoinMenuButtonCLicked();
	UFUNCTION()
	void OnJoinButtonCLicked();
	UFUNCTION()
	void OnRefreshButtonCLicked();
	UFUNCTION()
	void OnBackButtonCLicked();
	UFUNCTION()
	void OnQuitButtonCLicked();

	void UpdateChildrenVisual();
private:
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* Overlay_MainMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* Overlay_HostMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* Overlay_JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Host_Menu;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Host;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Join_Menu;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Quit;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ScrollBox_Sessions;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Join;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Refresh;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Back;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Back0;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Loading;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* EditableText_ServerName; 
	
	UPROPERTY(meta = (BindWidget))
	class UEditableText* EditableText_HostName;

	TSubclassOf<class UUserWidget> ServerRowClass;
	TOptional<uint32> SelectedIndex;
};
