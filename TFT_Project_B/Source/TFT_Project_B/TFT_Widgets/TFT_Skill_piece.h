#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFT_Skill_piece.generated.h"

enum WEAPON_TYPE
{
	closeRange,
	longLange
};


/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API UTFT_Skill_piece : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetSlotNumText(FString text);

	void SetSlotImg(WEAPON_TYPE itemtype, int32 socket);
	void ResetSlotImg();

	void SetSlotPBPercent(float percent);

	void SetCDT(float CDT);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillSlot, meta = (AllowPrivateAccess = "true"))
	bool bCoolDownOn = false;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillSlotNumText;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* SkillSlotPB;

	UPROPERTY(meta = (BindWidget))
	class UImage* SkillSlotImg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillSlot, meta = (AllowPrivateAccess = "true"))
	float _coolDownTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillSlot, meta = (AllowPrivateAccess = "true"))
	float _runTime = 0.0f;

private: // 선입력 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillSlot, meta = (AllowPrivateAccess = "true"))
	class UObject* _brushCloseQ_Resource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillSlot, meta = (AllowPrivateAccess = "true"))
	UObject* _brushCloseE_Resource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillSlot, meta = (AllowPrivateAccess = "true"))
	UObject* _brushLongQ_Resource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillSlot, meta = (AllowPrivateAccess = "true"))
	UObject* _brushLongE_Resource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillSlot, meta = (AllowPrivateAccess = "true"))
	UObject* _nullBrush_Resource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillSlot, meta = (AllowPrivateAccess = "true"))
	FSlateBrush _brushCloseQ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillSlot, meta = (AllowPrivateAccess = "true"))
	FSlateBrush _brushCloseE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillSlot, meta = (AllowPrivateAccess = "true"))
	FSlateBrush _brushLongQ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillSlot, meta = (AllowPrivateAccess = "true"))
	FSlateBrush _brushLongE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillSlot, meta = (AllowPrivateAccess = "true"))
	FSlateBrush _nullBrush;
	
};
