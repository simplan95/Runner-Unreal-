// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerObstacleSpawner.h"
#include "RunnerGameMode.h"
#include "RunnerFloor.h"
#include "RunnerObstacle.h"

#include "Engine/TargetPoint.h"//Ÿ������Ʈ Ŭ����
#include "Runtime/Engine/Public/EngineUtils.h"//���ͷ����� 

//��ֹ� ������
ARunnerObstacleSpawner::ARunnerObstacleSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));//��Ʈ ������
	check(Scene);
	RootComponent = Scene;

	SpawnTimer = 1.5f;//���� Ÿ�̸�

}

// Called when the game starts or when spawned
void ARunnerObstacleSpawner::BeginPlay()
{
	Super::BeginPlay();

	//���峻 Ÿ�� ����Ʈ ���ͷ����͸� ����Ͽ� Ÿ������Ʈ �迭 ����
	for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter)
	{
		SpawnTargets.Add(*TargetIter);
	}

	//���忡 �����ϴ� �÷ξ��� ų����Ʈ�� ��������Ʈ�� ���ε�
	for (TActorIterator<ARunnerFloor> FloorIter(GetWorld()); FloorIter; ++FloorIter)
	{
		if (FloorIter->GetWorld() == GetWorld())
		{
			KillPoint = FloorIter->GetKillPoint();
			SpawnPoint = FloorIter->GetSpawnPoint();
		}
	}
	TimeSinceLastSpawn = SpawnTimer;
	
}

// Called every frame
void ARunnerObstacleSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//��ֹ� ����ƽ ����
	TimeSinceLastSpawn += DeltaTime;

	//������ ������ ������ ��ֹ� ���� �ֱⰡ ���� ª����
	float TrueSpawnTime = 
		SpawnTimer / (float)GetCustomGameMode<ARunnerGameMode>(GetWorld())->GetGameLevel();

	if (TimeSinceLastSpawn > TrueSpawnTime)
	{
		TimeSinceLastSpawn = 0.0f;
		SpawnObstacle();
	}
}

void ARunnerObstacleSpawner::SpawnObstacle()
{
	if (SpawnTargets.Num() > 0 && ObstaclesToSpawn.Num() > 0)
	{
		//������ġ ����
		short SpawnerLocation = FMath::Rand() % SpawnTargets.Num();
		short Obstical = FMath::Rand() % ObstaclesToSpawn.Num();
		float CapsuleOffset = 0.0f;

		//���ͽ��� �Ķ���� ����
		FActorSpawnParameters SpawnInfo;

		//���� Ʈ������ ��������
		FTransform MyTrans = SpawnTargets[SpawnerLocation]->GetTransform();

		//���� ����Ʈ ����
		MyTrans.SetLocation(FVector(SpawnPoint, MyTrans.GetLocation().Y, MyTrans.GetLocation().Z));

		//��ֹ����� ����
		ARunnerObstacle* NewObs = GetWorld()->SpawnActor<ARunnerObstacle>(ObstaclesToSpawn[Obstical], MyTrans, SpawnInfo);

		//��ֹ� ���� ����
		if (NewObs)
		{
			//ų����Ʈ ����
			NewObs->SetKillPoint(KillPoint);

			//GetComponentByClass: �������� �Լ����� ������ �ڵ鿡�� ��ü ������Ʈ�� ������
			USphereComponent* ObsSphere = Cast<USphereComponent>(NewObs->GetComponentByClass(USphereComponent::StaticClass()));

			if (ObsSphere)
			{
				//��ֹ� ��ġ����
				NewObs->AddActorLocalOffset(FVector(0.0f, 0.0f, ObsSphere->GetUnscaledSphereRadius()));
			}
		}
	}
}
