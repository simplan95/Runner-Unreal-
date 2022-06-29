// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runner.h"
#include "RunnerCoin.h"//코인
#include "RunnerPowerUp.h"//파워업
#include "GameFramework/Actor.h"
#include "RunnerCoinSpawner.generated.h"

UCLASS()
class RUNNER_API ARunnerCoinSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARunnerCoinSpawner();


	UPROPERTY(EditAnywhere)
		TSubclassOf<ARunnerPowerUp> PowerUpObject;//파워업 클래스 추가(에디터 추가예정)

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))//에디터에서 파워업 등장확률설정 및 제한
		int32 PowerUpChance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnPowerUp();//파워업 오브젝트 생성

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<ARunnerCoin> CoinObject;//(에디터에서 블루프린트로 만든 객체를 추가함)

	UPROPERTY()
		TArray<class ATargetPoint*> SpawnTransforms;

	UPROPERTY()
		USceneComponent* Root;

	UPROPERTY(EditAnywhere)//최대코인셋
		int32 MaxSetCoins;

	UPROPERTY(EditAnywhere)//최소코인셋
		int32 MinSetCoins;

	UPROPERTY(EditAnywhere)//코인 셋 간격인터벌
		float CoinSetTimeInterval;

	UPROPERTY(EditAnywhere)//코인 셋 인터벌
		float CoinTimeInterval;

	UPROPERTY(EditAnywhere)//스폰경로 변경 인터벌
		float MovementTimeInterval;

protected:
	void SpawnCoin();
	void SpawnCoinSet();
	void MoveSpawner();

	int32 NumCoinsToSpawn;

	float KillPoint;
	float SpawnPoint;
	int32 TargetLoc;

	FTimerHandle CoinSetTimerHandle;//코인 셋 스폰 타이머
	FTimerHandle CoinTimerHandle;//코인 셋 간격 타이머
	FTimerHandle SpawnMoveTimerHandle;//스폰 경로 스위칭 타이머

};
