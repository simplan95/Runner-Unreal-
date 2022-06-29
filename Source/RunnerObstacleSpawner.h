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
	//장애물 스폰
	void SpawnObstacle();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)//장애물 배열 에디터에서 설정예정
		TArray<TSubclassOf<ARunnerObstacle>> ObstaclesToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)//타깃포인트배열
		TArray<class ATargetPoint*> SpawnTargets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpawnTimer;

	UPROPERTY()
		USceneComponent* Scene;

private://스포너에서 장애물의 킬,스폰포인트를 지정함
	float KillPoint;
	float SpawnPoint;
	float TimeSinceLastSpawn;

};
