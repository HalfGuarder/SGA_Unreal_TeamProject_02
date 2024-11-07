// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_AnimNotify_Effect.h"
#include "TFT_Characters/TFT_Monster.h"
#include "TFT_GameInstance.h"
#include "TFT_EffectManager.h"

void UTFT_AnimNotify_Effect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ATFT_Monster* Player = Cast<ATFT_Monster>(MeshComp->GetOwner());
		if (Player)
		{

			UTFT_GameInstance* GameInstance = Cast<UTFT_GameInstance>(Player->GetGameInstance());
			if (GameInstance && GameInstance->GetEffectManager())
			{

				FVector EffectLocation = Player->GetActorLocation() + Player->GetActorForwardVector() * 300.0f;
				FRotator EffectRotation = MeshComp->GetComponentRotation();


				GameInstance->GetEffectManager()->Play(TEXT("FirePunch"), 1, EffectLocation, EffectRotation);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("EffectManager or GameInstance is null"));
			}
		}
	}
}
