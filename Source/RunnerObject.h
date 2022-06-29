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
	// �⺻�� ����
	ARunnerObject();

protected:
	// ������ ȣ��
	virtual void BeginPlay() override;

public:	
	// �� ������ ȣ��
	virtual void Tick(float DeltaTime) override;

	float KillPoint;

	void SetKillPoint(float Point);//ų����Ʈ ����
	float GetKillPoint();//ų����Ʈ ��ȯ

protected:
	UFUNCTION()
		virtual void MyOnActorOverlap(AActor* OverlappedActor, AActor* otherActor);

	UFUNCTION()
		virtual void MyOnActorEndOverlap(AActor* OverlappedActor, AActor* otherActor);

	UPROPERTY(EditAnywhere)
		USphereComponent* Collider;
};
