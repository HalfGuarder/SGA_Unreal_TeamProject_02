// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFT_Effect.generated.h"

UCLASS()
class TFT_PROJECT_B_API ATFT_Effect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATFT_Effect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Play(int32 effectType, FVector location, FRotator rotator);

	bool IsPlaying();

	UFUNCTION()
	void EndParticle(class UParticleSystemComponent* particle);

	UFUNCTION()
	void EndNiagara(class UNiagaraComponent* niagara);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* _particleCom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UNiagaraComponent* _niagaraCom;
};
