// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TFT_StateComponent.generated.h"

UENUM()
enum class StateType : int32
{
	Airborne,
	Stun,
	Slow,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TFT_PROJECT_B_API UTFT_StateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTFT_StateComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SetState(StateType type);

	UFUNCTION()
	bool CheckState(StateType type);

	UFUNCTION()
	TArray<StateType> GetCurStates();

	UFUNCTION()
	void InitState();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	bool bIsAirborne = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	bool bIsStun = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	bool bIsSlow = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	TArray<StateType> _curStates;
		
};
