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

	void AButtonActor();

	virtual void Tick(float DeltaTime) override;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button")
    bool bIsOn;


    UFUNCTION(BlueprintCallable, Category = "Button")
    void ToggleButton();

    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBoxComponent* BoxComponent;

   
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* ButtonMesh;

    
    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAccess = "true"))
    FVector EffectLocationOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAccess = "true"))
    FRotator EffectRotationOffset;
    UPROPERTY()
    bool bPlayerIsNearby;


};
