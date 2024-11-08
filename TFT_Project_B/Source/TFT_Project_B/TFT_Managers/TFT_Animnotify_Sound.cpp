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
				// 사운드의 재생 위치와 회전 설정
				FVector SoundLocation = Player->GetActorLocation() + SoundLocationOffset;
				FRotator SoundRotation = MeshComp->GetComponentRotation() + SoundRotationOffset;

				// 사운드 매니저를 통해 사운드 재생
				GameInstance->GetSoundManager()->Play(TEXT("Sound"), SoundLocation, SoundRotation);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("SoundManager or GameInstance is null"));
			}
		}
	}
}