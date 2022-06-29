// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerCoin.h"
#include "RunnerGameMode.h"//게임모드
#include "RunnerCharacter.h"//러너캐릭터
#include "RunnerObstacle.h"//장애물

//점수 코인
ARunnerCoin::ARunnerCoin()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	check(Mesh);
	Mesh->AttachTo(RootComponent);
	Mesh->SetCollisionProfileName("OverlapAllDynamic");//콜리전 프로파일설정
}

void ARunnerCoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//메시회전
	Mesh->AddLocalRotation(FRotator(0.0f, 5.0f, 0.0f));
}

//코인이 장애물과 겹칠경우 코인을 장애물 위로 위치조정 
void ARunnerCoin::MyOnActorOverlap(AActor* OverlappedActor, AActor* otherActor)
{
	if (otherActor->GetClass()->IsChildOf(ARunnerObstacle::StaticClass()) && !BeingPulled)
	{
		//구체 스피어 컴포넌트를 가져옴 (장애물, 코인)
		USphereComponent* ThisSphere = Cast<USphereComponent>(GetComponentByClass(USphereComponent::StaticClass()));//코인
		USphereComponent* OtherSphere = Cast<USphereComponent>(otherActor->GetComponentByClass(USphereComponent::StaticClass()));//장애물

		if (OtherSphere)
		{
			AddActorLocalOffset(FVector(0.0f, 0.0f, (OtherSphere->GetUnscaledSphereRadius() * 2.0f) + Collider->GetUnscaledSphereRadius()));
		}
	}
	if (otherActor->GetClass()->IsChildOf(ARunnerCharacter::StaticClass()))//캐릭터일 경우 점수 Up
	{
		ARunnerCharacter* MyRunner = Cast<ARunnerCharacter>(otherActor);
		MyRunner->ScoreUp();
		GetWorld()->DestroyActor(this);
	}
}

