// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerObstacle.h"
#include "RunnerGameMode.h"

//��ֹ� ������Ʈ
ARunnerObstacle::ARunnerObstacle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	check(Mesh);
	Mesh->AttachTo(Collider);//�θ� ������ �ݶ��̴��� �޽ú���
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);//��� ä�ΰ� �浹 ����
}
