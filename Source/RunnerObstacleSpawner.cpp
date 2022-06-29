// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerObstacleSpawner.h"
#include "RunnerGameMode.h"
#include "RunnerFloor.h"
#include "RunnerObstacle.h"

#include "Engine/TargetPoint.h"//타겟포인트 클래스
#include "Runtime/Engine/Public/EngineUtils.h"//이터레이터 

//장애물 스포너
ARunnerObstacleSpawner::ARunnerObstacleSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));//루트 신지정
	check(Scene);
	RootComponent = Scene;

	SpawnTimer = 1.5f;//스폰 타이머

}

// Called when the game starts or when spawned
void ARunnerObstacleSpawner::BeginPlay()
{
	Super::BeginPlay();

	//월드내 타겟 포인트 이터레이터를 사용하여 타깃포인트 배열 설정
	for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter)
	{
		SpawnTargets.Add(*TargetIter);
	}

	//월드에 존재하는 플로어의 킬포인트와 스폰포인트를 바인딩
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

	//장애물 스폰틱 설정
	TimeSinceLastSpawn += DeltaTime;

	//레벨이 증가할 때마다 장애물 스폰 주기가 점점 짧아짐
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
		//스폰위치 설정
		short SpawnerLocation = FMath::Rand() % SpawnTargets.Num();
		short Obstical = FMath::Rand() % ObstaclesToSpawn.Num();
		float CapsuleOffset = 0.0f;

		//액터스폰 파라미터 선언
		FActorSpawnParameters SpawnInfo;

		//액터 트랜스폼 가져오기
		FTransform MyTrans = SpawnTargets[SpawnerLocation]->GetTransform();

		//스폰 포인트 지정
		MyTrans.SetLocation(FVector(SpawnPoint, MyTrans.GetLocation().Y, MyTrans.GetLocation().Z));

		//장애물액터 생성
		ARunnerObstacle* NewObs = GetWorld()->SpawnActor<ARunnerObstacle>(ObstaclesToSpawn[Obstical], MyTrans, SpawnInfo);

		//장애물 액터 설정
		if (NewObs)
		{
			//킬포인트 설정
			NewObs->SetKillPoint(KillPoint);

			//GetComponentByClass: 스폰액터 함수에서 가져온 핸들에서 구체 컴포넌트를 가져옴
			USphereComponent* ObsSphere = Cast<USphereComponent>(NewObs->GetComponentByClass(USphereComponent::StaticClass()));

			if (ObsSphere)
			{
				//장애물 위치조정
				NewObs->AddActorLocalOffset(FVector(0.0f, 0.0f, ObsSphere->GetUnscaledSphereRadius()));
			}
		}
	}
}
