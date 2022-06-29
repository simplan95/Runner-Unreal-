// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runner.h"
#include "RunnerCoin.h"//����
#include "RunnerPowerUp.h"//�Ŀ���
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
		TSubclassOf<ARunnerPowerUp> PowerUpObject;//�Ŀ��� Ŭ���� �߰�(������ �߰�����)

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))//�����Ϳ��� �Ŀ��� ����Ȯ������ �� ����
		int32 PowerUpChance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnPowerUp();//�Ŀ��� ������Ʈ ����

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<ARunnerCoin> CoinObject;//(�����Ϳ��� �������Ʈ�� ���� ��ü�� �߰���)

	UPROPERTY()
		TArray<class ATargetPoint*> SpawnTransforms;

	UPROPERTY()
		USceneComponent* Root;

	UPROPERTY(EditAnywhere)//�ִ����μ�
		int32 MaxSetCoins;

	UPROPERTY(EditAnywhere)//�ּ����μ�
		int32 MinSetCoins;

	UPROPERTY(EditAnywhere)//���� �� �������͹�
		float CoinSetTimeInterval;

	UPROPERTY(EditAnywhere)//���� �� ���͹�
		float CoinTimeInterval;

	UPROPERTY(EditAnywhere)//������� ���� ���͹�
		float MovementTimeInterval;

protected:
	void SpawnCoin();
	void SpawnCoinSet();
	void MoveSpawner();

	int32 NumCoinsToSpawn;

	float KillPoint;
	float SpawnPoint;
	int32 TargetLoc;

	FTimerHandle CoinSetTimerHandle;//���� �� ���� Ÿ�̸�
	FTimerHandle CoinTimerHandle;//���� �� ���� Ÿ�̸�
	FTimerHandle SpawnMoveTimerHandle;//���� ��� ����Ī Ÿ�̸�

};
