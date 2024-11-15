// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Managers/TFT_AnimNotify_RampageEffect.h"
#include "TFT_Characters/TFT_Monster.h"
#include "TFT_GameInstance.h"
#include "TFT_EffectManager.h"


void UTFT_AnimNotify_RampageEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ATFT_Monster* Player = Cast<ATFT_Monster>(MeshComp->GetOwner());
		if (Player)
		{
			UTFT_GameInstance* GameInstance = Cast<UTFT_GameInstance>(Player->GetGameInstance());
			if (GameInstance && GameInstance->GetEffectManager())
			{
				// 폭발 설정
				int NumRows = 3; // 앞으로 나가는 횟수
				int NumParticlesPerRow = 3; // 가로로 나가는 폭발 개수
				float RowInterval = 0.5f; // 각 행의 폭발 간 시간 간격
				float ForwardStep = 300.0f; // 각 행이 전진할 거리
				float HorizontalOffset = 200.0f; // 가로로 퍼질 거리 (좌, 중앙, 우로 분산)

				for (int row = 0; row < NumRows; ++row)
				{
					FTimerHandle TimerHandle;
					FTimerDelegate TimerDel;

					// 람다 함수를 사용하여 각 행의 폭발을 설정
					TimerDel.BindLambda([=]()
						{
							for (int i = 0; i < NumParticlesPerRow; ++i)
							{
								// 가로로 퍼질 위치 설정: 왼쪽, 중앙, 오른쪽
								float HorizontalPosition = (i - 1) * HorizontalOffset; // -1, 0, 1 위치로 분산

								// 폭발 위치 계산: 캐릭터 앞쪽으로 전진하면서 좌우로 퍼지도록 설정
								FVector EffectLocation = Player->GetActorLocation()
									+ Player->GetActorForwardVector() * (row + 1) * ForwardStep // 전진 거리
									+ Player->GetActorRightVector() * HorizontalPosition; // 좌우 퍼짐
								EffectLocation.Z -= 100.0f; // 지면과의 높이 조정

								// 폭발 실행
								GameInstance->GetEffectManager()->Play(TEXT("Explosion"), 0, EffectLocation, MeshComp->GetComponentRotation());
							}
						});

					// 각 행을 RowInterval 간격으로 순차적으로 실행
					Player->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, row * RowInterval, false);
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("EffectManager or GameInstance is null"));
			}
		}
	}
}
