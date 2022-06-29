// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runner.h"
#include "RunnerObject.h"
#include "RunnerPowerUp.generated.h"

/**
 * 
 */
UENUM()//블루프린트와 열거형의 호환
enum class EPowerUp : uint8//파워업 열거형
{
	SPEED = 1 UMETA(DisplayName = "Speed"),
	SMASH = 2 UMETA(DisplayName = "Smash"),
	MAGNET = 3 UMETA(DisplayName = "Magnet")
};

UCLASS()
class RUNNER_API ARunnerPowerUp : public ARunnerObject
{
	GENERATED_BODY()

public:
	ARunnerPowerUp();

	virtual void MyOnActorOverlap(AActor* OverlappedActor, AActor* otherActor) override;

protected:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

private:
	EPowerUp Type;
	EPowerUp GetType();
	void PowerUp();
	
};
