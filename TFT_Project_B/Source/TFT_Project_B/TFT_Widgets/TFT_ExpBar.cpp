// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Widgets/TFT_ExpBar.h"
#include "Components/ProgressBar.h"

void UTFT_ExpBar::SetExpBarValue(float ratio)
{
	PB_ExpBar->SetPercent(ratio);
}