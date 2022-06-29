// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerPowerUp.h"
#include "RunnerObstacle.h"//��ֹ�
#include "RunnerCharacter.h"//ĳ����

//ĳ���� �Ŀ��� ������
ARunnerPowerUp::ARunnerPowerUp()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	check(Mesh);

	//�޽� �ݸ�������
	Mesh->AttachTo(RootComponent);
	Mesh->SetCollisionProfileName("OverlapAllDynamic");

	PowerUp();//�Ŀ��� ����
	FString AssetName;

	switch (GetType())//�Ŀ���Ÿ�Կ� ���� ���� ������� 
	{
	case EPowerUp::SPEED:
		AssetName = "/Game/AssetForRunner/SM_PowerUp_Blue.SM_PowerUp_Blue";
		break;

	case EPowerUp::SMASH:
		AssetName = "/Game/AssetForRunner/SM_PowerUp_Green.SM_PowerUp_Green";
		break;

	case EPowerUp::MAGNET:
		AssetName = "/Game/AssetForRunner/SM_PowerUp_Red.SM_PowerUp_Red";
		break;
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> MyMesh(*AssetName);
	if (MyMesh.Succeeded())
	{
		Mesh->SetStaticMesh(MyMesh.Object);
	}

}

void ARunnerPowerUp::MyOnActorOverlap(AActor* OverlappedActor, AActor* otherActor)
{
	if (otherActor->GetClass()->IsChildOf(ARunnerObstacle::StaticClass()))//�������Ǵ� ���Ͱ� Obstacle �Ͻ�
	{
		USphereComponent* otherSphere = Cast<USphereComponent>(otherActor->GetComponentByClass(USphereComponent::StaticClass()));

		if (otherSphere)
		{
			AddActorLocalOffset(FVector(0.0f, 0.0f, (otherSphere->GetUnscaledSphereRadius()) + Collider->GetUnscaledSphereRadius() * 2.0f));
		}
	}

	if (otherActor->GetClass()->IsChildOf(ARunnerCharacter::StaticClass()))//�������Ǵ� ���Ͱ� Character �Ͻ�
	{
		ARunnerCharacter* thisChar = Cast<ARunnerCharacter>(otherActor);

		if (thisChar)
		{
			thisChar->PowerUp(GetType());
			GetWorld()->DestroyActor(this);
		}
	}
}

EPowerUp ARunnerPowerUp::GetType()
{
	return Type;
}

void ARunnerPowerUp::PowerUp()//�Ŀ��� ��������
{
	int iType = FMath::Rand() % 3;

	switch (iType)
	{
	case 0:
	{
		Type = EPowerUp::SPEED;
		break;
	}
	case 1:
	{
		Type = EPowerUp::SMASH;
		break;
	}
	case 2:
	{
		Type = EPowerUp::MAGNET;
		break;
	}
	default:
		break;
	}
}