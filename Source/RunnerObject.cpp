// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerObject.h"
#include "RunnerGameMode.h"//러너게임모드

//러너게임 오브젝트가 상속받을 클래스(코인, 장애물, 아이템)
ARunnerObject::ARunnerObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	check(Collider);

	RootComponent = Collider;
	Collider->SetCollisionProfileName("OverlapAllDynamic");//콜리전 프로파일설정
	Collider->SetSphereRadius(65.0f);

	OnActorBeginOverlap.AddDynamic(this, &ARunnerObject::MyOnActorOverlap);
	OnActorBeginOverlap.AddDynamic(this, &ARunnerObject::MyOnActorEndOverlap);
}

// Called when the game starts or when spawned
void ARunnerObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARunnerObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float gameSpeed = GetCustomGameMode<ARunnerGameMode>(GetWorld())->GetInvGameSpeed();

	AddActorWorldOffset(FVector(gameSpeed, 0.0f, 0.0f));//게임모드의 스피드만큼 해당 오브젝트를 이동함

	if (GetActorLocation().X < KillPoint)//킬포인트를 지나가면 액터 파괴
	{
		Destroy();
	}

}

void ARunnerObject::MyOnActorOverlap(AActor* OverlappedActor, AActor* otherActor)
{

}

void ARunnerObject::MyOnActorEndOverlap(AActor* OverlappedActor, AActor* otherActor)
{

}

void ARunnerObject::SetKillPoint(float point)
{
	KillPoint = point;
}

float ARunnerObject::GetKillPoint()
{
	return KillPoint;
}