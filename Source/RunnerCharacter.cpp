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
#include "Engine/TargetPoint.h"//Ÿ�� ����Ʈ
#include "Runtime/Engine/Public/EngineUtils.h"//���ͷ�����
#include "RunnerGameMode.h"//���� ���
#include "RunnerObstacle.h"//��ֹ�
#include "RunnerCoin.h"//����
#include "RunnerFloor.h"//�÷ξ�
#include "Sound/SoundCue.h"//���� ť
#include "RunnerPaticle_Explosive.h"//���� ��ƼŬ
#include "RunnerAnimInstance.h"//���ʾִ��ν��Ͻ�

//�÷��̾� ĳ���� ����, ��� ����
ARunnerCharacter::ARunnerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	//ī�޶�(��������)����, ����
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;	//ī�޶� �� ĳ���� ����
	CameraBoom->AddRelativeLocation(FVector(0.0f, 0.0f, 160.0f));	//�÷��̾���� ������

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetRelativeRotation(FRotator(-10.0f, 0.0f, 0.0f));	//ī�޶� ��������

	
	//�ִϸ��̼� ���� ������Ʈ ��� ����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> myMesh(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);//�ִϸ��̼� �߰�
	ConstructorHelpers::FClassFinder<UAnimInstance>myAnim(TEXT("/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C"));
	if (myAnim.Succeeded() && myMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(myMesh.Object);
		GetMesh()->SetAnimInstanceClass(myAnim.Class);
	}
	

	//ĳ���� �ʱ� ��ġ ����
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	//ĳ���� �̵�
	GetCharacterMovement()->JumpZVelocity = 1500.0f;
	GetCharacterMovement()->GravityScale = 4.5f;
	//ĳ���� �ٹٲ� �ӵ�
	CharSpeed = 10.0f;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARunnerCharacter::MyOnComponentOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ARunnerCharacter::MyOnComponentEndOverlap);

	// ID 0(�⺻ ��Ʈ�ѷ�)�� �Է� ��������
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//ĳ���� ���� ����(���� ȹ��, ���� ����)
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
	PlayerInputComponent->BindAction("Reset", IE_Pressed, this, &ARunnerCharacter::Reset).bExecuteWhenPaused = true;//������ �Ͻ� �����Ǿ ��ǲ�׼��� ����

}

void ARunnerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RunnerAnim = Cast<URunnerAnimInstance>(GetMesh()->GetAnimInstance());
}

void ARunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//���峻 Ÿ������Ʈ�� TargetArray�� ���ε�
	for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter)
	{
		TargetArray.Add(*TargetIter);//���忡 �ִ� ����Ʈ ������Ʈ�� ĳ���Ϳ� �߰�
	}

	//���忡 �ִ� �÷ξ��� ų����Ʈ�� ĳ���Ϳ� ����
	for (TActorIterator<ARunnerFloor> TargetIter(GetWorld()); TargetIter; ++TargetIter)
	{
		KillPoint = TargetIter->GetKillPoint();
	}

	//Ÿ������Ʈ ���� �� �߾� ��ġ(Y�� ����)
	auto SortPred = [](const AActor& A, const AActor& B)->bool
	{
		return(A.GetActorLocation().Y < B.GetActorLocation().Y);
	};

	TargetArray.Sort(SortPred);

	//�ʱ� ĳ���� ��ġ�� �߾�����
	CurrentLocation = ((TargetArray.Num() / 2) + (TargetArray.Num() % 2) - 1);
}

// Called every frame
void ARunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�� ƽ���� ĳ���� Ÿ����ġ �̵�
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

	//���� ���ǵ� ��ŭ ĳ���Ͱ� �з���
	if (bBeingPushed)
	{
		float MoveSpeed = GetCustomGameMode<ARunnerGameMode>(GetWorld())->GetInvGameSpeed();
		AddActorLocalOffset(FVector(MoveSpeed, 0.0f, 0.0f));
	}

	//���� ���
	if (CanMagnet)
	{
		CoinMagnet();
	}

	//ų����Ʈ ���޽� ų
	if (GetActorLocation().X < KillPoint)
	{
		GetCustomGameMode<ARunnerGameMode>(GetWorld())->GameOver();
	}
}

void ARunnerCharacter::MoveRight()//ĳ���� ���������� �̵�
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

void ARunnerCharacter::MoveLeft()//ĳ���� �������� �̵�
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
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("RunnerMap"));//���� ���� ���ؽ�Ʈ���� �־��� ������ ������ ����
}

void ARunnerCharacter::MyOnComponentOverlap(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->IsChildOf(ARunnerObstacle::StaticClass()))//��ֹ� ����ƽ�Ͻ� �ڷ� �з���
	{
		//��ֹ��� ��ġ�� ĳ������ ��ġ�� 60�� ���� �̸� ĳ���Ͱ� �ڷ� �з���
		FVector VecBetween = OtherActor->GetActorLocation() - GetActorLocation();
		float AngleBetween = FMath::Acos(FVector::DotProduct(VecBetween.GetSafeNormal(), GetActorForwardVector().GetSafeNormal()));

		AngleBetween *= (180 / PI);

		if (AngleBetween < 60.0f)
		{
			ARunnerObstacle* pObs = Cast<ARunnerObstacle>(OtherActor);

			if (pObs && CanSmash)//ĳ���Ͱ� ��ֹ��ı� ������ Ȱ��ȭ�� ��ֹ� �ı��� ����, ��ƼŬ ���
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
	//��ֹ��� �������� ������ �б� ��Ȱ��ȭ
	if (OtherActor->GetClass()->IsChildOf(ARunnerObstacle::StaticClass()))
	{
		bBeingPushed = false;
	}
}

void ARunnerCharacter::ScoreUp()//������ �� ���� ��� �� ���Ӹ���� ���ھ� ��
{
	dingSound->Play();
	Score++;
	GetCustomGameMode<ARunnerGameMode>(GetWorld())->CharScoreUp(Score);
}

int32 ARunnerCharacter::GetScore()
{
	return Score;
}

void ARunnerCharacter::PowerUp(EPowerUp Type)//�Ŀ��� ������ ���� �Ŀ��� ����
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

void ARunnerCharacter::CoinMagnet()//���忡�ִ� ������ ĳ���Ϳ� �����Ÿ� ���� �Ͻ� ���׳� Ȱ��ȭ
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