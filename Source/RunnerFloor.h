// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runner.h"
#include "GameFramework/Actor.h"
#include "RunnerFloor.generated.h"

UCLASS()
class RUNNER_API ARunnerFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARunnerFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetKillPoint();//ų����Ʈ 
	float GetSpawnPoint();//��������Ʈ 

protected:
	UPROPERTY(EditAnywhere)
		TArray<USceneComponent*> FloorMeshScenes;

	UPROPERTY(EditAnywhere)
		TArray<UStaticMeshComponent*> FloorMeshes;

	UPROPERTY(EditAnywhere)
		UBoxComponent* CollisionBox;

	int32 NumRepeatingMesh;//�ٴڸ޽� �ݺ� ��
	float KillPoint;//ų����Ʈ(X ��ǥ)
	float SpawnPoint;//��������Ʈ(X ��ǥ)

};
