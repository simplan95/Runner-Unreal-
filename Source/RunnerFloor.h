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

	float GetKillPoint();//킬포인트 
	float GetSpawnPoint();//스폰포인트 

protected:
	UPROPERTY(EditAnywhere)
		TArray<USceneComponent*> FloorMeshScenes;

	UPROPERTY(EditAnywhere)
		TArray<UStaticMeshComponent*> FloorMeshes;

	UPROPERTY(EditAnywhere)
		UBoxComponent* CollisionBox;

	int32 NumRepeatingMesh;//바닥메시 반복 수
	float KillPoint;//킬포인트(X 좌표)
	float SpawnPoint;//스폰포인트(X 좌표)

};
