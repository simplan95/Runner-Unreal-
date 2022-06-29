// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerObject.h"
#include "RunnerGameMode.h"//���ʰ��Ӹ��

//���ʰ��� ������Ʈ�� ��ӹ��� Ŭ����(����, ��ֹ�, ������)
ARunnerObject::ARunnerObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	check(Collider);

	RootComponent = Collider;
	Collider->SetCollisionProfileName("OverlapAllDynamic");//�ݸ��� �������ϼ���
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

	AddActorWorldOffset(FVector(gameSpeed, 0.0f, 0.0f));//���Ӹ���� ���ǵ常ŭ �ش� ������Ʈ�� �̵���

	if (GetActorLocation().X < KillPoint)//ų����Ʈ�� �������� ���� �ı�
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