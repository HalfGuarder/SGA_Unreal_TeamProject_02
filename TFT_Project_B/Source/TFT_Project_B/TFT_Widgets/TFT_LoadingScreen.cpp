// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Widgets/TFT_LoadingScreen.h"

bool UTFT_LoadingScreen::Initialize()
{
    bool Success = Super::Initialize();

    return Success;
}

void UTFT_LoadingScreen::StartEvent()
{
}

void UTFT_LoadingScreen::ExitEvent()
{
}

void UTFT_LoadingScreen::HideUI()
{
    this->RemoveFromParent();
}
