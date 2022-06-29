// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerObstacle.h"
#include "RunnerGameMode.h"

//장애물 오브젝트
ARunnerObstacle::ARunnerObstacle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	check(Mesh);
	Mesh->AttachTo(Collider);//부모 변수인 콜라이더에 메시부착
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);//모든 채널과 충돌 무시
}
