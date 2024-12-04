// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFT_Button.generated.h"

UCLASS()
class TFT_PROJECT_B_API ATFT_Button : public AActor
{
	GENERATED_BODY()

public:
	ATFT_Button();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// 버튼 활성화
	UFUNCTION(BlueprintCallable, Category = "Button")
	void ToggleButton();

public:
	// 이펙트 활성화 관련 함수
	void ActivateEnergyEffect();
	void ActivateAttackUpEffect();

	// 보스 데미지 처리
	void ApplyDamageToBoss();


	// 콜리전 관리
	void EnableEffectCollision();
	void DisableEffectCollision();

	// 쿨다운 처리
	void StartCooldown();
	void EndCooldown();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	// 타이머 핸들
	FTimerHandle EnergyEffectTimerHandle;
	FTimerHandle AttackUpEffectTimerHandle;
	FTimerHandle DamageTimerHandle;
	FTimerHandle CooldownTimerHandle;

	// 콜리전 관련 이벤트
	UFUNCTION()
	void OnEffectOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// 버튼 상태 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button", meta = (AllowPrivateAccess = "true"))
	bool bIsOn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cooldown", meta = (AllowPrivateAccess = "true"))
	bool bIsCooldown;

	// 보스가 콜리전에 있는지 확인
	bool bBossInCollision;

	// 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ButtonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect Collision", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* EffectCollision;

	// 이펙트 위치/회전
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	FVector EffectLocationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	FRotator EffectRotationOffset;



	bool bPlayerNearby;
};