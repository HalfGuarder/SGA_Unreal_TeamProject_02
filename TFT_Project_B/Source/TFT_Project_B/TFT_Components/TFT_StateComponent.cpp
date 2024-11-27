// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_StateComponent.h"

UTFT_StateComponent::UTFT_StateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UTFT_StateComponent::BeginPlay()
{
	Super::BeginPlay();


}

void UTFT_StateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UTFT_StateComponent::SetState(StateType type)
{
	switch (type)
	{
	case StateType::Airborne:
		bIsAirborne = true;
		_curStates.Add(StateType::Airborne);
		UE_LOG(LogTemp, Log, TEXT("SetAirborne"));
		break;

	case StateType::Stun:
		bIsStun = true;
		_curStates.Add(StateType::Stun);
		UE_LOG(LogTemp, Log, TEXT("SetStun"));
		break;

	case StateType::Slow:
		bIsSlow = true;
		_curStates.Add(StateType::Slow);
		UE_LOG(LogTemp, Log, TEXT("SetSlow"));
		break;

	default:
		break;
	}

	_stateChangeDelegate.Broadcast();
}

bool UTFT_StateComponent::CheckState(StateType type)
{
	switch (type)
	{
	case StateType::Airborne:
		return bIsAirborne;

	case StateType::Stun:
		return bIsStun;

	case StateType::Slow:
		return bIsSlow;

	default:
		return false;
	}
}

TArray<StateType> UTFT_StateComponent::GetCurStates()
{
	return _curStates;
}

void UTFT_StateComponent::InitState()
{
	bIsAirborne = false;
	bIsStun = false;
	bIsSlow = false;

	_curStates.Empty();
}
