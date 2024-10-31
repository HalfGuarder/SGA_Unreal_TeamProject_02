// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_GameModeBase.h"

#include "TFT_Player.h"

ATFT_GameModeBase::ATFT_GameModeBase()
{
	static ConstructorHelpers::FClassFinder<ATFT_Player> player
	(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Characters/Player/TFT_Player_BP.TFT_Player_BP_C'"));

	if (player.Succeeded())
	{
		DefaultPawnClass = player.Class;
	}
}

void ATFT_GameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void ATFT_GameModeBase::BeginPlay()
{
	Super::BeginPlay();

}
