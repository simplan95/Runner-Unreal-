// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerCoin.h"
#include "RunnerGameMode.h"//���Ӹ��
#include "RunnerCharacter.h"//����ĳ����
#include "RunnerObstacle.h"//��ֹ�

//���� ����
ARunnerCoin::ARunnerCoin()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	check(Mesh);
	Mesh->AttachTo(RootComponent);
	Mesh->SetCollisionProfileName("OverlapAllDynamic");//�ݸ��� �������ϼ���
}

void ARunnerCoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�޽�ȸ��
	Mesh->AddLocalRotation(FRotator(0.0f, 5.0f, 0.0f));
}

//������ ��ֹ��� ��ĥ��� ������ ��ֹ� ���� ��ġ���� 
void ARunnerCoin::MyOnActorOverlap(AActor* OverlappedActor, AActor* otherActor)
{
	if (otherActor->GetClass()->IsChildOf(ARunnerObstacle::StaticClass()) && !BeingPulled)
	{
		//��ü ���Ǿ� ������Ʈ�� ������ (��ֹ�, ����)
		USphereComponent* ThisSphere = Cast<USphereComponent>(GetComponentByClass(USphereComponent::StaticClass()));//����
		USphereComponent* OtherSphere = Cast<USphereComponent>(otherActor->GetComponentByClass(USphereComponent::StaticClass()));//��ֹ�

		if (OtherSphere)
		{
			AddActorLocalOffset(FVector(0.0f, 0.0f, (OtherSphere->GetUnscaledSphereRadius() * 2.0f) + Collider->GetUnscaledSphereRadius()));
		}
	}
	if (otherActor->GetClass()->IsChildOf(ARunnerCharacter::StaticClass()))//ĳ������ ��� ���� Up
	{
		ARunnerCharacter* MyRunner = Cast<ARunnerCharacter>(otherActor);
		MyRunner->ScoreUp();
		GetWorld()->DestroyActor(this);
	}
}

