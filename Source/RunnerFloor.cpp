// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerFloor.h"
#include "RunnerGameMode.h"

//���� �÷��� �ٴ�
ARunnerFloor::ARunnerFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//��Ʈ ������Ʈ����
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	//�޽� �� ���׸��� ���۷���
	ConstructorHelpers::FObjectFinder<UStaticMesh>myMesh(TEXT("/Game/AssetForRunner/Floor_Mesh_Ruuner.Floor_Mesh_Ruuner"));
	//ConstructorHelpers::FObjectFinder<UMaterial>myMaterial(TEXT("/Game/BountyDash/Concrete.Concrete"));

	//�÷ξ� ����
	if (myMesh.Succeeded())
	{
		NumRepeatingMesh = 80;//�޽� ���� �ݺ���

		FBoxSphereBounds myBounds = myMesh.Object->GetBounds();
		float XBounds = myBounds.BoxExtent.X * 2;//�������� �ǳ� ���� �����̹Ƿ� 2��� ����
		float ScenePos = ((XBounds * (NumRepeatingMesh - 1)) / 2.0f) * -1;//����ü�� ������ ������ ����ȭ �Ͽ� ���� ����

		KillPoint = ScenePos - (XBounds * 0.5f);//�������� X�ٿ���� ���ݸ�ŭ �� ���� ų����Ʈ�� ����
		SpawnPoint = (ScenePos * -1) + (XBounds * 0.5f);//�������� X�ٿ���� ���ݸ�ŭ ���� ���� ��������Ʈ�μ���

		for (int i = 0; i < NumRepeatingMesh; ++i)
		{
			//�� �ʱ�ȭ
			FString SceneName = "Scene" + FString::FromInt(i);
			FName SceneID = FName(*SceneName);
			USceneComponent* thisScene = CreateDefaultSubobject<USceneComponent>(SceneID);
			check(thisScene);

			//�ش� ���� ��Ʈ������ ������ �� ���������� �޽���ġ�� X�ٿ�常ŭ ���Ͽ� ����
			thisScene->AttachTo(RootComponent);
			thisScene->SetRelativeLocation(FVector(ScenePos, 0.0f, 0.0f));
			ScenePos += XBounds;

			FloorMeshScenes.Add(thisScene);

			//�޽��ʱ�ȭ
			FString MeshName = "Mesh" + FString::FromInt(i);
			UStaticMeshComponent* thisMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(*MeshName));
			check(thisMesh);

			//�޽ø� �ش� �ſ� ���� �� �ݸ�������
			thisMesh->AttachTo(FloorMeshScenes[i]);
			thisMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
			thisMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
			thisMesh->SetStaticMesh(myMesh.Object);

			/*
			if (myMaterial.Succeeded())
			{
				thisMesh->SetMaterial(0, myMaterial.Object);
			}
			*/
			FloorMeshes.Add(thisMesh);
		}

		//�ݸ����ڽ� ����
		CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
		check(CollisionBox);

		CollisionBox->AttachTo(RootComponent);
		CollisionBox->SetBoxExtent(FVector(SpawnPoint, myBounds.BoxExtent.Y, myBounds.BoxExtent.Z));
		CollisionBox->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	}
}

float ARunnerFloor::GetKillPoint()
{
	return KillPoint;
}

float ARunnerFloor::GetSpawnPoint()
{
	return SpawnPoint;
}

// Called when the game starts or when spawned
void ARunnerFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARunnerFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (auto Scene : FloorMeshScenes)
	{
		//��� �÷ξ ų����Ʈ�� �̵�
		Scene->AddLocalOffset(FVector(GetCustomGameMode<ARunnerGameMode>(GetWorld())->GetInvGameSpeed(), 0.0f, 0.0f));

		if (Scene->GetComponentTransform().GetLocation().X <= KillPoint)//�÷ξ ų����Ʈ���޽� 
		{
			//�÷ξ�޽� ��������Ʈ �̵�
			Scene->SetRelativeLocation(FVector(SpawnPoint, 0.0f, 0.0f));
		}
	}
}

