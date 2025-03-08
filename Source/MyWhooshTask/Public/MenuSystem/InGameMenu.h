// Copyright Huda Rasmey. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

/**
 *
 */
UCLASS()
class MYWHOOSHTASK_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	virtual void Setup() override;

	void OpenPauseScreen();
	void OpenCustomizationScreen();

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void OnQuitButtonCLicked();
	UFUNCTION()
	void OnCancelButtonCLicked_PauseMenu();

	UFUNCTION()
	void OnCancelButtonCLicked_CustomizationMenu();

	UFUNCTION()
	void OnColorButtonCLicked_Blue();

	UFUNCTION()
	void OnColorButtonCLicked_Red();

	UFUNCTION()
	void OnColorButtonCLicked_Green();

	UFUNCTION()
	void OnColorButtonCLicked_White();

private:
	UPROPERTY(meta = (BindWidget))
	class UOverlay* Overlay_Pause;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* Overlay_Customization;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Quit;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Cancel_Pause;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Cancel_Customization;

	UPROPERTY(meta = (BindWidget))
	class UColoringButton* Button_Blue;

	UPROPERTY(meta = (BindWidget))
	class UColoringButton* Button_Red;

	UPROPERTY(meta = (BindWidget))
	class UColoringButton* Button_Green;

	UPROPERTY(meta = (BindWidget))
	class UColoringButton* Button_White;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_Info;
};
