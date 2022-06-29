// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerCoinSpawner.h"
#include "Engine/TargetPoint.h" //Ÿ������Ʈ
#include "Runtime/Engine/Public/EngineUtils.h"//���ͷ�����
#include "Runtime/Engine/Public/TimerManager.h"//Ÿ�̸ӸŴ���
#include "RunnerCoin.h"//����
#include "RunnerPowerUp.h"//�Ŀ���
#include "RunnerFloor.h"//�÷ξ�

//���� �� �������� �����ϴ� ������
ARunnerCoinSpawner::ARunnerCoinSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//��Ʈ�� ����
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	//���� ��Ʈ �� ���� �� Ÿ�̸� ����
	MaxSetCoins = 5;//���� �ִ� ����
	MinSetCoins = 3;//���� �ּ� ����
	CoinSetTimeInterval = 4.0;//���� ���� ���� ���͹�
	CoinTimeInterval = 0.5;//���� ���� ���� ���͹�
	MovementTimeInterval = 1.0f;//���� ��ġ ���� ���͹�
}

// Called when the game starts or when spawned
void ARunnerCoinSpawner::BeginPlay()
{
	Super::BeginPlay();

	//���� Ÿ������Ʈ�� ���ε� 
	for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter)
	{
		SpawnTransforms.Add(*TargetIter);
	}

	//���忡 �����ϴ� �÷ξ��� ų,���� ����Ʈ�� ���ε�
	for (TActorIterator<ARunnerFloor> FloorIter(GetWorld()); FloorIter; ++FloorIter)
	{
		if (FloorIter->GetWorld() == GetWorld())
		{
			KillPoint = FloorIter->GetKillPoint();
			SpawnPoint = FloorIter->GetSpawnPoint();
		}
	}

	//Ÿ�̸� ����
	FTimerManager& WorldTimeManager = GetWorld()->GetTimerManager();

	//SetTimer(TimerHandle, �����Ǵ� �Լ��� ȣ���� ������Ʈ, Ÿ�̸ӿϷ�� ������ �Լ�, Ÿ�̸Ӱ���, �ݺ�����)
	//SpawnCoinSet�� ȣ��Ǹ� CoinSetTimerHandle�� Ŭ������ �� CoinTimerHandle�� ���� �� SpawnCoin�� ȣ���ϰ�
	//  SpawnCoin�Ϸ�� CoinTimerHandle Ŭ���� �� CoinSetTimerHandle�� ���� ����
	WorldTimeManager.SetTimer(CoinSetTimerHandle, this, &ARunnerCoinSpawner::SpawnCoinSet, CoinSetTimeInterval, false);
	WorldTimeManager.SetTimer(SpawnMoveTimerHandle, this, &ARunnerCoinSpawner::MoveSpawner, MovementTimeInterval, true);
	
}

// Called every frame
void ARunnerCoinSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARunnerCoinSpawner::SpawnCoin()
{
	//�����Ϳ��� ������ Ȯ���� �Ŀ����� ����
	if (FMath::Rand() % 100 < PowerUpChance)
	{
		SpawnPowerUp();
		NumCoinsToSpawn--;
	}
	else
	{
		//���ͽ��� �Ķ���� ����
		FActorSpawnParameters SpawnParams;

		//���� Ʈ������ ��������
		FTransform CoinTransform = SpawnTransforms[TargetLoc]->GetTransform();

		//���� ����Ʈ ����
		CoinTransform.SetLocation(FVector(SpawnPoint, CoinTransform.GetLocation().Y, CoinTransform.GetLocation().Z));

		//���� ����
		ARunnerCoin* SpawnedCoin = GetWorld()->SpawnActor<ARunnerCoin>(CoinObject, CoinTransform, SpawnParams);

		if (SpawnedCoin)
		{
			//ų����Ʈ ����
			SpawnedCoin->SetKillPoint(KillPoint);

			USphereComponent* CoinSphere = Cast<USphereComponent>(SpawnedCoin->GetComponentByClass(USphereComponent::StaticClass()));

			if (CoinSphere)
			{
				float Offset = CoinSphere->GetUnscaledSphereRadius();
				SpawnedCoin->AddActorLocalOffset(FVector(0.0f, 0.0f, Offset));
			}
			NumCoinsToSpawn--;
		}
	}
	if (NumCoinsToSpawn <= 0)//NumCoinsToSpawn�� 0�̵Ǹ� ���� Ÿ�̸Ӹ� Ŭ������ �ٸ� Ÿ�̸� ����
	{
		FTimerManager& WorldTimeManager = GetWorld()->GetTimerManager();
		WorldTimeManager.SetTimer(CoinSetTimerHandle, this, &ARunnerCoinSpawner::SpawnCoinSet, CoinSetTimeInterval, false);
		WorldTimeManager.ClearTimer(CoinTimerHandle);
	}

}

//���ι��� ����
void ARunnerCoinSpawner::SpawnCoinSet()
{
	NumCoinsToSpawn = FMath::RandRange(MinSetCoins, MaxSetCoins);

	FTimerManager& WorldTimeManager = GetWorld()->GetTimerManager();

	//Ȱ���� Ÿ�̸� ��ü
	WorldTimeManager.ClearTimer(CoinSetTimerHandle);

	WorldTimeManager.SetTimer(CoinTimerHandle, this, &ARunnerCoinSpawner::SpawnCoin, CoinTimeInterval, true);
}

void ARunnerCoinSpawner::MoveSpawner()//��������Ʈ�� ����
{
	TargetLoc = FMath::Rand() % SpawnTransforms.Num();
}

//�Ŀ��� ����
void ARunnerCoinSpawner::SpawnPowerUp()
{

	FActorSpawnParameters SpawnInfo;//���ͽ��� �Ķ���� ����

	FTransform MyTrans = SpawnTransforms[TargetLoc]->GetTransform();//���� Ʈ������ ��������

	MyTrans.SetLocation(FVector(SpawnPoint, MyTrans.GetLocation().Y, MyTrans.GetLocation().Z));//���� ����Ʈ ����

	ARunnerPowerUp* NewPowerUp = GetWorld()->SpawnActor<ARunnerPowerUp>(PowerUpObject, MyTrans, SpawnInfo);//�Ŀ��� ����

	if (NewPowerUp)
	{
		NewPowerUp->SetKillPoint(KillPoint);//ų����Ʈ ����

		USphereComponent* PowerUpSphere = Cast<USphereComponent>(NewPowerUp->GetComponentByClass(USphereComponent::StaticClass()));

		if (PowerUpSphere)
		{
			float Offset = PowerUpSphere->GetUnscaledSphereRadius();
			NewPowerUp->AddActorLocalOffset(FVector(0.0f, 0.0f, Offset));
		}
	}
}
