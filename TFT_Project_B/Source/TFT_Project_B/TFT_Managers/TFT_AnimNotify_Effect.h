// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "TFT_AnimNotify_Effect.generated.h"

/**
 *
 */
UCLASS()
class TFT_PROJECT_B_API UTFT_AnimNotify_Effect : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FString EffectName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FVector EffectLocationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FRotator EffectRotationOffset;
};