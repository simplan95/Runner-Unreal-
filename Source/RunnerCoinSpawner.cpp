// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerCoinSpawner.h"
#include "Engine/TargetPoint.h" //타깃포인트
#include "Runtime/Engine/Public/EngineUtils.h"//이터레이터
#include "Runtime/Engine/Public/TimerManager.h"//타이머매니저
#include "RunnerCoin.h"//코인
#include "RunnerPowerUp.h"//파워업
#include "RunnerFloor.h"//플로어

//코인 및 아이템을 스폰하는 스포너
ARunnerCoinSpawner::ARunnerCoinSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//루트신 설정
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	//코인 세트 수 제한 및 타이머 설정
	MaxSetCoins = 5;//코인 최대 묶음
	MinSetCoins = 3;//코인 최소 묶음
	CoinSetTimeInterval = 4.0;//코인 묶음 스폰 인터벌
	CoinTimeInterval = 0.5;//각각 코인 스폰 인터벌
	MovementTimeInterval = 1.0f;//스폰 위치 변경 인터벌
}

// Called when the game starts or when spawned
void ARunnerCoinSpawner::BeginPlay()
{
	Super::BeginPlay();

	//월드 타깃포인트와 바인딩 
	for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter)
	{
		SpawnTransforms.Add(*TargetIter);
	}

	//월드에 존재하는 플로어의 킬,스폰 포인트를 바인딩
	for (TActorIterator<ARunnerFloor> FloorIter(GetWorld()); FloorIter; ++FloorIter)
	{
		if (FloorIter->GetWorld() == GetWorld())
		{
			KillPoint = FloorIter->GetKillPoint();
			SpawnPoint = FloorIter->GetSpawnPoint();
		}
	}

	//타이머 생성
	FTimerManager& WorldTimeManager = GetWorld()->GetTimerManager();

	//SetTimer(TimerHandle, 제공되는 함수를 호출할 오브젝트, 타이머완료시 실행할 함수, 타이머간격, 반복여부)
	//SpawnCoinSet이 호출되면 CoinSetTimerHandle을 클리어한 후 CoinTimerHandle를 셋한 뒤 SpawnCoin을 호출하고
	//  SpawnCoin완료시 CoinTimerHandle 클리어 및 CoinSetTimerHandle을 새로 셋함
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
	//에디터에서 설정한 확률로 파워업을 스폰
	if (FMath::Rand() % 100 < PowerUpChance)
	{
		SpawnPowerUp();
		NumCoinsToSpawn--;
	}
	else
	{
		//액터스폰 파라미터 선언
		FActorSpawnParameters SpawnParams;

		//액터 트랜스폼 가져오기
		FTransform CoinTransform = SpawnTransforms[TargetLoc]->GetTransform();

		//스폰 포인트 지정
		CoinTransform.SetLocation(FVector(SpawnPoint, CoinTransform.GetLocation().Y, CoinTransform.GetLocation().Z));

		//코인 스폰
		ARunnerCoin* SpawnedCoin = GetWorld()->SpawnActor<ARunnerCoin>(CoinObject, CoinTransform, SpawnParams);

		if (SpawnedCoin)
		{
			//킬포인트 설정
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
	if (NumCoinsToSpawn <= 0)//NumCoinsToSpawn가 0이되면 코인 타이머를 클리어후 다른 타이머 생성
	{
		FTimerManager& WorldTimeManager = GetWorld()->GetTimerManager();
		WorldTimeManager.SetTimer(CoinSetTimerHandle, this, &ARunnerCoinSpawner::SpawnCoinSet, CoinSetTimeInterval, false);
		WorldTimeManager.ClearTimer(CoinTimerHandle);
	}

}

//코인묶음 스폰
void ARunnerCoinSpawner::SpawnCoinSet()
{
	NumCoinsToSpawn = FMath::RandRange(MinSetCoins, MaxSetCoins);

	FTimerManager& WorldTimeManager = GetWorld()->GetTimerManager();

	//활성된 타이머 교체
	WorldTimeManager.ClearTimer(CoinSetTimerHandle);

	WorldTimeManager.SetTimer(CoinTimerHandle, this, &ARunnerCoinSpawner::SpawnCoin, CoinTimeInterval, true);
}

void ARunnerCoinSpawner::MoveSpawner()//스폰포인트를 변경
{
	TargetLoc = FMath::Rand() % SpawnTransforms.Num();
}

//파워업 스폰
void ARunnerCoinSpawner::SpawnPowerUp()
{

	FActorSpawnParameters SpawnInfo;//액터스폰 파라미터 선언

	FTransform MyTrans = SpawnTransforms[TargetLoc]->GetTransform();//액터 트랜스폼 가져오기

	MyTrans.SetLocation(FVector(SpawnPoint, MyTrans.GetLocation().Y, MyTrans.GetLocation().Z));//스폰 포인트 지정

	ARunnerPowerUp* NewPowerUp = GetWorld()->SpawnActor<ARunnerPowerUp>(PowerUpObject, MyTrans, SpawnInfo);//파워업 생성

	if (NewPowerUp)
	{
		NewPowerUp->SetKillPoint(KillPoint);//킬포인트 설정

		USphereComponent* PowerUpSphere = Cast<USphereComponent>(NewPowerUp->GetComponentByClass(USphereComponent::StaticClass()));

		if (PowerUpSphere)
		{
			float Offset = PowerUpSphere->GetUnscaledSphereRadius();
			NewPowerUp->AddActorLocalOffset(FVector(0.0f, 0.0f, Offset));
		}
	}
}
