// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Animnotify_Sound.h"
#include "TFT_Characters/TFT_Monster.h"
#include "TFT_GameInstance.h"
#include "TFT_SoundManager.h"

void UTFT_Animnotify_Sound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ATFT_Monster* Player = Cast<ATFT_Monster>(MeshComp->GetOwner());
		if (Player)
		{
			UTFT_GameInstance* GameInstance = Cast<UTFT_GameInstance>(Player->GetGameInstance());
			if (GameInstance && GameInstance->GetSoundManager())
			{
				
				FVector SoundLocation = Player->GetActorLocation() + SoundLocationOffset;
				FRotator SoundRotation = MeshComp->GetComponentRotation() + SoundRotationOffset;

				
				GameInstance->GetSoundManager()->PlaySound(TEXT("Explosion"), SoundLocation, SoundRotation);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("SoundManager or GameInstance is null"));
			}
		}
	}
}