// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Runner.h"
#include "RunnerPowerUp.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"//����� ������Ʈ �߰�
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
	// ������ ���۵ǰų� �����Ǿ��� �� ȣ���
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:
	// �� ������ ȣ��� 
	virtual void Tick(float DeltaTime) override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	//���ھ��/���ھ� ��ȯ
	void ScoreUp();
	int32 GetScore();

	//�Ŀ���
	void PowerUp(EPowerUp Type);

protected:
	// �÷��̾� �¿��� Ÿ�� ��ġ �̵��� �����Ѵ� 
	void MoveRight();
	void MoveLeft();

	UFUNCTION()
		void Reset();

	// ������Ʈ �ݸ��� ĸ���� ���Ǵ� ������ �Լ� 
	UFUNCTION()
		void MyOnComponentOverlap(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void MyOnComponentEndOverlap(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// �̵� ���� �迭
	UPROPERTY(EditAnywhere, Category = Logic)
		TArray<class ATargetPoint*> TargetArray;

	// ĳ���� �� �ٲ� �ӵ� 
	UPROPERTY(EditAnywhere, Category = Logic)
		float CharSpeed;

	// ���� ȹ�� ���带 ���� ����� ������Ʈ
	UPROPERTY(EditAnywhere, Category = Sound)
		UAudioComponent* dingSound;

	// ��ֹ� �ı� ���带 ���� ����� ������Ʈ
	UPROPERTY(EditAnywhere, Category = Sound)
		UAudioComponent* explosionSound;
	// �÷��̾� ���� 
	UPROPERTY(BlueprintReadOnly)
		int32 Score;

	UPROPERTY()
		class URunnerAnimInstance* RunnerAnim;//�ִ��ν��Ͻ� Ŭ���� ����


	//�Ŀ��� ���� ����/�Լ� (�����Ϳ��� ��������)
	UPROPERTY(EditAnywhere, Category = PowerUps)
		float SmashTime;//Smash �ð�
	
	UPROPERTY(EditAnywhere, Category = PowerUps)
		float MagnetTime;//Magnet�ð�

	UPROPERTY(EditAnywhere, Category = PowerUps)
		float MagnetReach;//Magnet����

	UFUNCTION()
		void StopSmash();

	UFUNCTION()
		void StopMagnet();

	void CoinMagnet();

private:
	// ĳ���� �� ��ġ ������ 
	short CurrentLocation;
	FVector DesiredLocation;
	bool bBeingPushed;

	//ĳ���� ų����Ʈ
	float KillPoint;

	//�Ŀ��� �Ӽ�
	bool CanSmash;
	bool CanMagnet;

};

