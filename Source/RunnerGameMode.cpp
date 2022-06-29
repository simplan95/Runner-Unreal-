// Copyright Epic Games, Inc. All Rights Reserved.

#include "RunnerGameMode.h"
#include "RunnerCharacter.h"
#include "RunnerHUD.h"//러너HUD
#include "UObject/ConstructorHelpers.h"

//게임 모드(게임 스피드, 레벨,게임 오버)
ARunnerGameMode::ARunnerGameMode()
{
	// set default pawn class to our Blueprinted character
	//러너캐릭터를 기본 캐릭터로 설정
	DefaultPawnClass = ARunnerCharacter::StaticClass();
	//기본 UHD로 설정
	HUDClass = ARunnerHUD::StaticClass();

	////게임 스피드 및 레벨
	numCoinsForSpeedIncrease = 5;
	gameSpeed = 10.0f;
	gameSpeedIncrease = 5.0f;
	gameLevel = 1;

	////게임 시간 및 게임오버확인
	RunTime = 0.0f;
	bGameOver = false;
	StartGameOverCount = false;
	TimeTillGameOver = 2.0f;
	GameOverTimer = 0.0f;
}

void ARunnerGameMode::CharScoreUp(unsigned int charScore)
{
	//일정수의 코인을 얻을 때 마다 레벨 및 스피드증가
	if (charScore != 0 && charScore % numCoinsForSpeedIncrease == 0)
	{
		gameSpeed += gameSpeedIncrease;
		gameLevel++;
	}
}

float ARunnerGameMode::GetInvGameSpeed()
{
	return -gameSpeed;//게임속도 반환
}

float ARunnerGameMode::GetGameSpeed()
{
	return gameSpeed;//게임속도 반환
}

int32 ARunnerGameMode::GetGameLevel()
{
	return gameLevel;//게임레벨 반환
}

void ARunnerGameMode::ReduceGameSpeed()//게임 스피드 및 레벨 다운
{
	//게임 속도가 10이상 일 때만 스피드 및 레벨 다운 가능
	if (gameSpeed > 10.0f)
	{
		gameSpeed -= gameSpeedIncrease;
		gameLevel--;
	}
}

//캐릭터 게임오버를 확인한 후 2초뒤 게임오버
void ARunnerGameMode::Tick(float DeltaTime)
{
	if (!StartGameOverCount)//게임오버 전까지 플레이타임 저장
	{
		RunTime += DeltaTime;
	}
	else//게임 오버 후 2초 뒤 bGameOver true(게임정지, 결과표시)
	{
		GameOverTimer += DeltaTime;
		if (GameOverTimer >= TimeTillGameOver)
		{
			bGameOver = true;
		}
	}
}

void ARunnerGameMode::SetGamePaused(bool GamePaused)//게임오버시 플레이어 퓨즈
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

