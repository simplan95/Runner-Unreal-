// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runner.h"
#include "RunnerObject.h"
#include "RunnerCoin.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API ARunnerCoin : public ARunnerObject
{
	GENERATED_BODY()
	
		
public:
	ARunnerCoin();

	// �������� ȣ��
	virtual void Tick(float DeltaTime) override;

	virtual void MyOnActorOverlap(AActor* OverlappedActor, AActor* otherActor) override;

	//���� �ڼ� 
	bool BeingPulled;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Mesh;
		

};
