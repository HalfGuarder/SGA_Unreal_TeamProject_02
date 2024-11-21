#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFT_GameStartWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartEvent);

UCLASS()
class TFT_PROJECT_B_API UTFT_GameStartWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void StartEvent();
	UFUNCTION()
	void ExitEvent();


	UPROPERTY(BlueprintAssignable, Category = "Event")
	FStartEvent _StartEvent;

private:

	//UPROPERTY(meta = (BindWidget))
	//class UButton* GameStart_Button;

	//UPROPERTY(meta = (BindWidget))
	//UButton* Exit_Button;

	void HideUI();
};
