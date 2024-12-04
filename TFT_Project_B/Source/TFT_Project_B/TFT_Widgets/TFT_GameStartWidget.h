#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFT_GameStartWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartPageEvent);

UCLASS()
class TFT_PROJECT_B_API UTFT_GameStartWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	UFUNCTION()
	void StartEvent();
	UFUNCTION()
	void ExitEvent();


	UPROPERTY(BlueprintAssignable, Category = "Event")
	FStartPageEvent _StartEvent;

private:
	void HideUI();
};
