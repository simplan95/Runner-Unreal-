// Copyright Epic Games, Inc. All Rights Reserved.

#include "RunnerCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Animation/AnimInstance.h" 
#include "Animation/AnimBlueprint.h" 
#include "Engine/TargetPoint.h"//타깃 포인트
#include "Runtime/Engine/Public/EngineUtils.h"//이터레이터
#include "RunnerGameMode.h"//게임 모드
#include "RunnerObstacle.h"//장애물
#include "RunnerCoin.h"//코인
#include "RunnerFloor.h"//플로어
#include "Sound/SoundCue.h"//사운드 큐
#include "RunnerPaticle_Explosive.h"//폭발 파티클
#include "RunnerAnimInstance.h"//러너애님인스턴스

//플레이어 캐릭터 설정, 기능 구현
ARunnerCharacter::ARunnerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	//카메라(스프링암)생성, 설정
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;	//카메라 와 캐릭터 간격
	CameraBoom->AddRelativeLocation(FVector(0.0f, 0.0f, 160.0f));	//플레이어로의 오프셋

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetRelativeRotation(FRotator(-10.0f, 0.0f, 0.0f));	//카메라 각도설정

	
	//애니메이션 관련 오브젝트 경로 설정
	ConstructorHelpers::FObjectFinder<USkeletalMesh> myMesh(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);//애니메이션 추가
	ConstructorHelpers::FClassFinder<UAnimInstance>myAnim(TEXT("/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C"));
	if (myAnim.Succeeded() && myMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(myMesh.Object);
		GetMesh()->SetAnimInstanceClass(myAnim.Class);
	}
	

	//캐릭터 초기 위치 설정
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	//캐릭터 이동
	GetCharacterMovement()->JumpZVelocity = 1500.0f;
	GetCharacterMovement()->GravityScale = 4.5f;
	//캐릭터 줄바꿈 속도
	CharSpeed = 10.0f;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARunnerCharacter::MyOnComponentOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ARunnerCharacter::MyOnComponentEndOverlap);

	// ID 0(기본 컨트롤러)의 입력 가져오기
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//캐릭터 사운드 설정(코인 획득, 폭발 사운드)
	dingSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Ding"));
	dingSound->bAutoActivate = false;
	dingSound->AttachTo(RootComponent);
	ConstructorHelpers::FObjectFinder<USoundCue>myDingCue(TEXT("/Game/AssetForRunner/Sound/SC_Ding.SC_Ding"));
	if (myDingCue.Succeeded())
	{
		dingSound->SetSound(myDingCue.Object);
	}

	explosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Explosion"));
	explosionSound->bAutoActivate = false;
	explosionSound->AttachTo(RootComponent);
	ConstructorHelpers::FObjectFinder<USoundCue>myExplosionCue(TEXT("/Game/AssetForRunner/Sound/SC_Explosion.SC_Explosion"));
	if (myExplosionCue.Succeeded())
	{
		explosionSound->SetSound(myExplosionCue.Object);
	}

}

//////////////////////////////////////////////////////////////////////////
// Input

void ARunnerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ARunnerCharacter::MoveRight);
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ARunnerCharacter::MoveLeft);
	PlayerInputComponent->BindAction("Reset", IE_Pressed, this, &ARunnerCharacter::Reset).bExecuteWhenPaused = true;//게임이 일시 중지되어도 인풋액션을 수행

}

void ARunnerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RunnerAnim = Cast<URunnerAnimInstance>(GetMesh()->GetAnimInstance());
}

void ARunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//월드내 타겟포인트와 TargetArray와 바인딩
	for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter)
	{
		TargetArray.Add(*TargetIter);//월드에 있는 포인트 오브젝트를 캐릭터에 추가
	}

	//월드에 있는 플로어의 킬포인트를 캐릭터에 설정
	for (TActorIterator<ARunnerFloor> TargetIter(GetWorld()); TargetIter; ++TargetIter)
	{
		KillPoint = TargetIter->GetKillPoint();
	}

	//타깃포인트 정렬 및 중앙 위치(Y축 기준)
	auto SortPred = [](const AActor& A, const AActor& B)->bool
	{
		return(A.GetActorLocation().Y < B.GetActorLocation().Y);
	};

	TargetArray.Sort(SortPred);

	//초기 캐릭터 위치를 중앙으로
	CurrentLocation = ((TargetArray.Num() / 2) + (TargetArray.Num() % 2) - 1);
}

// Called every frame
void ARunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//매 틱마다 캐릭터 타겟위치 이동
	if (TargetArray.Num() > 0)
	{
		FVector TargetLoc = TargetArray[CurrentLocation]->GetActorLocation();
		TargetLoc.Z = GetActorLocation().Z;
		TargetLoc.X = GetActorLocation().X;

		if (TargetLoc != GetActorLocation())
		{
			SetActorLocation(FMath::Lerp(GetActorLocation(), TargetLoc, CharSpeed * DeltaTime));
		}
	}

	//게임 스피드 만큼 캐릭터가 밀려남
	if (bBeingPushed)
	{
		float MoveSpeed = GetCustomGameMode<ARunnerGameMode>(GetWorld())->GetInvGameSpeed();
		AddActorLocalOffset(FVector(MoveSpeed, 0.0f, 0.0f));
	}

	//코인 흡수
	if (CanMagnet)
	{
		CoinMagnet();
	}

	//킬포인트 도달시 킬
	if (GetActorLocation().X < KillPoint)
	{
		GetCustomGameMode<ARunnerGameMode>(GetWorld())->GameOver();
	}
}

void ARunnerCharacter::MoveRight()//캐릭터 오른쪽으로 이동
{
	if (Controller != nullptr)
	{
		if (CurrentLocation < TargetArray.Num() - 1)
		{
			++CurrentLocation;
		}
		else
		{
			//Do Nothing
		}
	}
}

void ARunnerCharacter::MoveLeft()//캐릭터 왼쪽으로 이동
{
	if (Controller != nullptr)
	{
		if (CurrentLocation > 0)
		{
			--CurrentLocation;
		}
		else
		{
			//Do Nothing
		}
	}
}

void ARunnerCharacter::Reset()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("RunnerMap"));//게임 월드 컨텍스트에서 주어진 지정된 레벨을 오픈
}

void ARunnerCharacter::MyOnComponentOverlap(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->IsChildOf(ARunnerObstacle::StaticClass()))//장애물 스태틱일시 뒤로 밀려남
	{
		//장애물의 위치와 캐릭터의 위치가 60도 이하 이면 캐릭터가 뒤로 밀려남
		FVector VecBetween = OtherActor->GetActorLocation() - GetActorLocation();
		float AngleBetween = FMath::Acos(FVector::DotProduct(VecBetween.GetSafeNormal(), GetActorForwardVector().GetSafeNormal()));

		AngleBetween *= (180 / PI);

		if (AngleBetween < 60.0f)
		{
			ARunnerObstacle* pObs = Cast<ARunnerObstacle>(OtherActor);

			if (pObs && CanSmash)//캐릭터가 장애물파괴 아이템 활성화시 장애물 파괴및 사운드, 파티클 재생
			{
				pObs->Destroy();//

				explosionSound->Play();

				ARunnerPaticle_Explosive* Paticle =
				GetWorld()->SpawnActor<ARunnerPaticle_Explosive>(ARunnerPaticle_Explosive::StaticClass(), pObs->GetTransform());
				Paticle->SetKillPoint(KillPoint);

			}
			else
			{
				bBeingPushed = true;
			}
		}
	}
}

void ARunnerCharacter::MyOnComponentEndOverlap(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//장애물과 오버랩이 끝나면 밀기 비활성화
	if (OtherActor->GetClass()->IsChildOf(ARunnerObstacle::StaticClass()))
	{
		bBeingPushed = false;
	}
}

void ARunnerCharacter::ScoreUp()//아이템 겟 사운드 재생 및 게임모드의 스코어 업
{
	dingSound->Play();
	Score++;
	GetCustomGameMode<ARunnerGameMode>(GetWorld())->CharScoreUp(Score);
}

int32 ARunnerCharacter::GetScore()
{
	return Score;
}

void ARunnerCharacter::PowerUp(EPowerUp Type)//파워업 종류에 따라 파워업 설정
{
	dingSound->Play();

	switch (Type)
	{
	case EPowerUp::SPEED:
	{
		GetCustomGameMode<ARunnerGameMode>(GetWorld())->ReduceGameSpeed();
		break;
	}
	case EPowerUp::SMASH:
	{
		CanSmash = true;
		FTimerHandle SmashTimer;
		GetWorld()->GetTimerManager().SetTimer(SmashTimer, this, &ARunnerCharacter::StopSmash, SmashTime, false);
		break;
	}
	case EPowerUp::MAGNET:
	{
		CanMagnet = true;
		FTimerHandle MagnetTimer;
		GetWorld()->GetTimerManager().SetTimer(MagnetTimer, this, &ARunnerCharacter::StopMagnet, MagnetTime, false);
		break;
	}
	default:
		break;
	}

}

void ARunnerCharacter::StopSmash()
{
	CanSmash = false;
}

void ARunnerCharacter::StopMagnet()
{
	CanMagnet = false;
}

void ARunnerCharacter::CoinMagnet()//워드에있는 코인을 캐릭터와 일정거리 이하 일시 마그넷 활성화
{
	for (TActorIterator<ARunnerCoin> coinIter(GetWorld()); coinIter; ++coinIter)
	{
		FVector Between = GetActorLocation() - coinIter->GetActorLocation();
		if (FMath::Abs(Between.Size()) < MagnetReach)
		{
			FVector CoinPos = FMath::Lerp(coinIter->GetActorLocation(), GetActorLocation(), 0.2f);
			coinIter->SetActorLocation(CoinPos);
			coinIter->BeingPulled = true;
		}
	}
}