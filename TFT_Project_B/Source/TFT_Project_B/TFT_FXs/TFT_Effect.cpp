// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Effect.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

// Sets default values
ATFT_Effect::ATFT_Effect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	_particleCom = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	RootComponent = _particleCom;

	_niagaraCom = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	_niagaraCom->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATFT_Effect::BeginPlay()
{
	Super::BeginPlay();

	_particleCom->OnSystemFinished.AddDynamic(this, &ATFT_Effect::EndParticle);
	EndParticle(_particleCom);

	_niagaraCom->OnSystemFinished.AddDynamic(this, &ATFT_Effect::EndNiagara);
	EndNiagara(_niagaraCom);
	
}

// Called every frame
void ATFT_Effect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATFT_Effect::Play(int32 effectType, FVector location, FRotator rotator)
{
	SetActorLocationAndRotation(location, rotator);

	if (effectType == 0)
	{
		_particleCom->ActivateSystem();
	}
	else if (effectType == 1)
	{
		_niagaraCom->ActivateSystem();
	}
}

bool ATFT_Effect::IsPlaying()
{
	if (_particleCom->IsActive()) return true;

	if (_niagaraCom->IsActive()) return true;

	return false;
}

void ATFT_Effect::EndParticle(UParticleSystemComponent* particle)
{
	if (particle) particle->Deactivate();
}

void ATFT_Effect::EndNiagara(UNiagaraComponent* niagara)
{
	if (niagara) niagara->Deactivate();
}

