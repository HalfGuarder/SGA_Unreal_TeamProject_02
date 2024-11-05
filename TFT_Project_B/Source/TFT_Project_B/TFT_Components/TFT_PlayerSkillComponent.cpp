// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Components/TFT_PlayerSkillComponent.h"

UTFT_PlayerSkillComponent::UTFT_PlayerSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UTFT_PlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UTFT_PlayerSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UTFT_PlayerSkillComponent::Q_Skill()
{

}

