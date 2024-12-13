// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Managers/TFT_Animnotity_PlayerEffect.h"
#include "TFT_Characters/TFT_Monster.h"
#include "TFT_GameInstance.h"
#include "TFT_EffectManager.h"
#include "TFT_Characters/TFT_Player.h"

void UTFT_Animnotity_PlayerEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ATFT_Player* Player = Cast<ATFT_Player>(MeshComp->GetOwner());
		if (Player)
		{

			UTFT_GameInstance* GameInstance = Cast<UTFT_GameInstance>(Player->GetGameInstance());
			if (GameInstance && GameInstance->GetEffectManager())
			{

				FVector EffectLocation = Player->GetActorLocation() + Player->GetActorForwardVector() * 200.0f;
				FRotator EffectRotation = MeshComp->GetComponentRotation();


				EFFECTMANAGER->Play(TEXT("TrunAttack"), 1, EffectLocation, EffectRotation);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("EffectManager or GameInstance is null"));
			}
		}
	}
}
