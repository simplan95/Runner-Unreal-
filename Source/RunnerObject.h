// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runner.h"
#include "GameFramework/Actor.h"
#include "RunnerObject.generated.h"

UCLASS()
class RUNNER_API ARunnerObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// 기본값 설정
	ARunnerObject();

protected:
	// 스폰시 호출
	virtual void BeginPlay() override;

public:	
	// 매 프레임 호출
	virtual void Tick(float DeltaTime) override;

	float KillPoint;

	void SetKillPoint(float Point);//킬포인트 설정
	float GetKillPoint();//킬포인트 반환

protected:
	UFUNCTION()
		virtual void MyOnActorOverlap(AActor* OverlappedActor, AActor* otherActor);

	UFUNCTION()
		virtual void MyOnActorEndOverlap(AActor* OverlappedActor, AActor* otherActor);

	UPROPERTY(EditAnywhere)
		USphereComponent* Collider;
};
