// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Widgets/TFT_HPBarWidget.h"
#include "Components/ProgressBar.h"

void UTFT_HPBarWidget::SetHpBarValue(float ratio)
{
	BP_HpBar->SetPercent(ratio);
}

void UTFT_HPBarWidget::SetMpBarValue(float ratio)
{
	//PB_MpBar->SetPercent(ratio);
}

