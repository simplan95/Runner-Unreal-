// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerPowerUp.h"
#include "RunnerObstacle.h"//장애물
#include "RunnerCharacter.h"//캐릭터

//캐릭터 파워업 아이템
ARunnerPowerUp::ARunnerPowerUp()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	check(Mesh);

	//메시 콜리전설정
	Mesh->AttachTo(RootComponent);
	Mesh->SetCollisionProfileName("OverlapAllDynamic");

	PowerUp();//파워업 지정
	FString AssetName;

	switch (GetType())//파워업타입에 따라 에셋 경로지정 
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
	if (otherActor->GetClass()->IsChildOf(ARunnerObstacle::StaticClass()))//오버랩되는 액터가 Obstacle 일시
	{
		USphereComponent* otherSphere = Cast<USphereComponent>(otherActor->GetComponentByClass(USphereComponent::StaticClass()));

		if (otherSphere)
		{
			AddActorLocalOffset(FVector(0.0f, 0.0f, (otherSphere->GetUnscaledSphereRadius()) + Collider->GetUnscaledSphereRadius() * 2.0f));
		}
	}

	if (otherActor->GetClass()->IsChildOf(ARunnerCharacter::StaticClass()))//오버랩되는 액터가 Character 일시
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

void ARunnerPowerUp::PowerUp()//파워업 랜덤설정
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