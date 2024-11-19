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
				
				int NumRows = 3; 
				int NumParticlesPerRow = 3; 
				float RowInterval = 0.5f; 
				float ForwardStep = 300.0f;
				float HorizontalOffset = 200.0f; 

				for (int row = 0; row < NumRows; ++row)
				{
					FTimerHandle TimerHandle;
					FTimerDelegate TimerDel;

					
					TimerDel.BindLambda([=]()
						{
							for (int i = 0; i < NumParticlesPerRow; ++i)
							{
						
								float HorizontalPosition = (i - 1) * HorizontalOffset; 

								
								FVector EffectLocation = Player->GetActorLocation()
									+ Player->GetActorForwardVector() * (row + 1) * ForwardStep 
									+ Player->GetActorRightVector() * HorizontalPosition; 
								EffectLocation.Z -= 100.0f; 

								
								GameInstance->GetEffectManager()->Play(TEXT("Explosion"), 0, EffectLocation, MeshComp->GetComponentRotation());
							}
						});

					
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
