// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runner.h"
#include "RunnerObstacle.h"
#include "GameFramework/Actor.h"
#include "RunnerObstacleSpawner.generated.h"

UCLASS()
class RUNNER_API ARunnerObstacleSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARunnerObstacleSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	//��ֹ� ����
	void SpawnObstacle();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)//��ֹ� �迭 �����Ϳ��� ��������
		TArray<TSubclassOf<ARunnerObstacle>> ObstaclesToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)//Ÿ������Ʈ�迭
		TArray<class ATargetPoint*> SpawnTargets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpawnTimer;

	UPROPERTY()
		USceneComponent* Scene;

private://�����ʿ��� ��ֹ��� ų,��������Ʈ�� ������
	float KillPoint;
	float SpawnPoint;
	float TimeSinceLastSpawn;

};
