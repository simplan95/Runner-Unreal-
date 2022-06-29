// Copyright Epic Games, Inc. All Rights Reserved.

#include "RunnerGameMode.h"
#include "RunnerCharacter.h"
#include "RunnerHUD.h"//����HUD
#include "UObject/ConstructorHelpers.h"

//���� ���(���� ���ǵ�, ����,���� ����)
ARunnerGameMode::ARunnerGameMode()
{
	// set default pawn class to our Blueprinted character
	//����ĳ���͸� �⺻ ĳ���ͷ� ����
	DefaultPawnClass = ARunnerCharacter::StaticClass();
	//�⺻ UHD�� ����
	HUDClass = ARunnerHUD::StaticClass();

	////���� ���ǵ� �� ����
	numCoinsForSpeedIncrease = 5;
	gameSpeed = 10.0f;
	gameSpeedIncrease = 5.0f;
	gameLevel = 1;

	////���� �ð� �� ���ӿ���Ȯ��
	RunTime = 0.0f;
	bGameOver = false;
	StartGameOverCount = false;
	TimeTillGameOver = 2.0f;
	GameOverTimer = 0.0f;
}

void ARunnerGameMode::CharScoreUp(unsigned int charScore)
{
	//�������� ������ ���� �� ���� ���� �� ���ǵ�����
	if (charScore != 0 && charScore % numCoinsForSpeedIncrease == 0)
	{
		gameSpeed += gameSpeedIncrease;
		gameLevel++;
	}
}

float ARunnerGameMode::GetInvGameSpeed()
{
	return -gameSpeed;//���Ӽӵ� ��ȯ
}

float ARunnerGameMode::GetGameSpeed()
{
	return gameSpeed;//���Ӽӵ� ��ȯ
}

int32 ARunnerGameMode::GetGameLevel()
{
	return gameLevel;//���ӷ��� ��ȯ
}

void ARunnerGameMode::ReduceGameSpeed()//���� ���ǵ� �� ���� �ٿ�
{
	//���� �ӵ��� 10�̻� �� ���� ���ǵ� �� ���� �ٿ� ����
	if (gameSpeed > 10.0f)
	{
		gameSpeed -= gameSpeedIncrease;
		gameLevel--;
	}
}

//ĳ���� ���ӿ����� Ȯ���� �� 2�ʵ� ���ӿ���
void ARunnerGameMode::Tick(float DeltaTime)
{
	if (!StartGameOverCount)//���ӿ��� ������ �÷���Ÿ�� ����
	{
		RunTime += DeltaTime;
	}
	else//���� ���� �� 2�� �� bGameOver true(��������, ���ǥ��)
	{
		GameOverTimer += DeltaTime;
		if (GameOverTimer >= TimeTillGameOver)
		{
			bGameOver = true;
		}
	}
}

void ARunnerGameMode::SetGamePaused(bool GamePaused)//���ӿ����� �÷��̾� ǻ��
{
	APlayerController* myPlayer = GetWorld()->GetFirstPlayerController();
	if (myPlayer != nullptr)
	{
		myPlayer->SetPause(GamePaused);
	}
}

float ARunnerGameMode::GetRunTime()
{
	return RunTime;
}

bool ARunnerGameMode::GetGameOver()
{
	return bGameOver;
}

void ARunnerGameMode::GameOver()
{
	StartGameOverCount = true;
}

