// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerFloor.h"
#include "RunnerGameMode.h"

//게임 플레이 바닥
ARunnerFloor::ARunnerFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//루트 컴포넌트설정
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	//메시 및 마테리알 레퍼런스
	ConstructorHelpers::FObjectFinder<UStaticMesh>myMesh(TEXT("/Game/AssetForRunner/Floor_Mesh_Ruuner.Floor_Mesh_Ruuner"));
	//ConstructorHelpers::FObjectFinder<UMaterial>myMaterial(TEXT("/Game/BountyDash/Concrete.Concrete"));

	//플로어 설정
	if (myMesh.Succeeded())
	{
		NumRepeatingMesh = 80;//메시 생성 반복수

		FBoxSphereBounds myBounds = myMesh.Object->GetBounds();
		float XBounds = myBounds.BoxExtent.X * 2;//중점에서 맨끝 까지 기준이므로 2배로 보완
		float ScenePos = ((XBounds * (NumRepeatingMesh - 1)) / 2.0f) * -1;//신전체의 길이의 절반을 음수화 하여 원점 설정

		KillPoint = ScenePos - (XBounds * 0.5f);//원점에서 X바운드의 절반만큼 뺀 곳을 킬포인트로 설정
		SpawnPoint = (ScenePos * -1) + (XBounds * 0.5f);//원점에서 X바운드의 절반만큼 더한 곳을 스폰포인트로설정

		for (int i = 0; i < NumRepeatingMesh; ++i)
		{
			//신 초기화
			FString SceneName = "Scene" + FString::FromInt(i);
			FName SceneID = FName(*SceneName);
			USceneComponent* thisScene = CreateDefaultSubobject<USceneComponent>(SceneID);
			check(thisScene);

			//해당 신을 루트컴포에 부착한 후 신포스부터 메시위치를 X바운드만큼 더하여 생성
			thisScene->AttachTo(RootComponent);
			thisScene->SetRelativeLocation(FVector(ScenePos, 0.0f, 0.0f));
			ScenePos += XBounds;

			FloorMeshScenes.Add(thisScene);

			//메시초기화
			FString MeshName = "Mesh" + FString::FromInt(i);
			UStaticMeshComponent* thisMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(*MeshName));
			check(thisMesh);

			//메시를 해당 신에 부착 및 콜리전설정
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

		//콜리전박스 설정
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
		//모든 플로어가 킬포인트로 이동
		Scene->AddLocalOffset(FVector(GetCustomGameMode<ARunnerGameMode>(GetWorld())->GetInvGameSpeed(), 0.0f, 0.0f));

		if (Scene->GetComponentTransform().GetLocation().X <= KillPoint)//플로어가 킬포인트도달시 
		{
			//플로어메시 스폰포인트 이동
			Scene->SetRelativeLocation(FVector(SpawnPoint, 0.0f, 0.0f));
		}
	}
}

