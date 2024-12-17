// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Managers/TFT_AnimNotify_PlayerSound.h"
#include "TFT_Characters/TFT_Monster.h"
#include "TFT_GameInstance.h"
#include "TFT_SoundManager.h"

void UTFT_AnimNotify_PlayerSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ATFT_Creature* Player = Cast<ATFT_Creature>(MeshComp->GetOwner());
		if (Player)
		{
			UTFT_GameInstance* GameInstance = Cast<UTFT_GameInstance>(Player->GetGameInstance());
			if (GameInstance && GameInstance->GetSoundManager())
			{

				FVector SoundLocation = Player->GetActorLocation() + SoundLocationOffset;
				FRotator SoundRotation = MeshComp->GetComponentRotation() + SoundRotationOffset;


				GameInstance->GetSoundManager()->PlaySound(TEXT("P_EskillSound"), SoundLocation, SoundRotation);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("SoundManager or GameInstance is null"));
			}
		}
	}
}
