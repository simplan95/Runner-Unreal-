// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Runner.h"
#include "GameFramework/GameModeBase.h"
#include "RunnerGameMode.generated.h"

UCLASS(minimalapi)
class ARunnerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARunnerGameMode();


public:
	void CharScoreUp(unsigned int charScore);//ĳ���� ���ھ� ��

	UFUNCTION()
		float GetInvGameSpeed();//���Ӽӵ� ��ȯ(-)

	UFUNCTION()
		float GetGameSpeed();//���Ӽӵ���ȯ(+)

	UFUNCTION()
		int32 GetGameLevel();//���ӷ�����ȯ

	void ReduceGameSpeed();//���� �ӵ� ���̱�

protected:
	UPROPERTY()//���ӽ��ǵ�
		float gameSpeed;

	UPROPERTY()//���ӷ���
		int32 gameLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)//�ӵ������� ���� �����ʿ��
		int32 numCoinsForSpeedIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)//���ӽ��ǵ� ������
		float gameSpeedIncrease;



public:
	UPROPERTY()//��Ÿ��
		float RunTime;

	UFUNCTION()//ƽ
		virtual void Tick(float DeltaTime) override;

	UFUNCTION()//���ӿ�������
		bool GetGameOver();

	UFUNCTION()//���ӿ���
		void GameOver();

	UFUNCTION()//�Ͻ�����
		void SetGamePaused(bool GamePaused);

	UPROPERTY()//���ӿ��� ����
		bool bGameOver;

	UPROPERTY()//���� ��Ÿ�� ī��Ʈ
		bool StartGameOverCount;

	UPROPERTY()//���ӿ����� �����ð�
		float TimeTillGameOver;

	UPROPERTY()//���ӿ����� Ÿ�̸�
		float GameOverTimer;

	float GetRunTime();//���� ��Ÿ�� ��ȯ
};



