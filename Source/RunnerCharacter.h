// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Runner.h"
#include "RunnerPowerUp.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"//오디오 컴포넌트 추가
#include "GameFramework/Character.h"
#include "RunnerCharacter.generated.h"

UCLASS(config=Game)
class ARunnerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ARunnerCharacter();

protected:
	// 게임이 시작되거나 스폰되었을 때 호출됨
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:
	// 매 프레임 호출됨 
	virtual void Tick(float DeltaTime) override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	//스코어업/스코어 반환
	void ScoreUp();
	int32 GetScore();

	//파워업
	void PowerUp(EPowerUp Type);

protected:
	// 플레이어 좌우의 타겟 위치 이동을 조정한다 
	void MoveRight();
	void MoveLeft();

	UFUNCTION()
		void Reset();

	// 컴포넌트 콜리전 캡슐에 사용되는 오버랩 함수 
	UFUNCTION()
		void MyOnComponentOverlap(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void MyOnComponentEndOverlap(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// 이동 방향 배열
	UPROPERTY(EditAnywhere, Category = Logic)
		TArray<class ATargetPoint*> TargetArray;

	// 캐릭터 줄 바꿈 속도 
	UPROPERTY(EditAnywhere, Category = Logic)
		float CharSpeed;

	// 코인 획득 사운드를 위한 오디오 컴포넌트
	UPROPERTY(EditAnywhere, Category = Sound)
		UAudioComponent* dingSound;

	// 장애물 파괴 사운드를 위한 오디오 컴포넌트
	UPROPERTY(EditAnywhere, Category = Sound)
		UAudioComponent* explosionSound;
	// 플레이어 점수 
	UPROPERTY(BlueprintReadOnly)
		int32 Score;

	UPROPERTY()
		class URunnerAnimInstance* RunnerAnim;//애님인스턴스 클래스 선언


	//파워업 관련 변수/함수 (에디터에서 설정예정)
	UPROPERTY(EditAnywhere, Category = PowerUps)
		float SmashTime;//Smash 시간
	
	UPROPERTY(EditAnywhere, Category = PowerUps)
		float MagnetTime;//Magnet시간

	UPROPERTY(EditAnywhere, Category = PowerUps)
		float MagnetReach;//Magnet범위

	UFUNCTION()
		void StopSmash();

	UFUNCTION()
		void StopMagnet();

	void CoinMagnet();

private:
	// 캐릭터 신 위치 데이터 
	short CurrentLocation;
	FVector DesiredLocation;
	bool bBeingPushed;

	//캐릭터 킬포인트
	float KillPoint;

	//파워업 속성
	bool CanSmash;
	bool CanMagnet;

};

