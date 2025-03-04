// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.h"
#include "ServerRow.generated.h"

/**
 *
 */
UCLASS()
class MYWHOOSHTASK_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup(FServerData ServerData, class UMainMenu* InMainMenu, uint32 InItemIndex);

private:
	UFUNCTION()
	void OnItemButtonCLicked();

public:
	UPROPERTY(BlueprintReadOnly)
	bool Selected = false;

	uint32 ItemIndex;
private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_ServerName; 
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_PlayersNumber; 
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Host;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Item;

	class UMainMenu* MainMenu;

};
