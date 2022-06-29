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
	void CharScoreUp(unsigned int charScore);//캐릭터 스코어 업

	UFUNCTION()
		float GetInvGameSpeed();//게임속도 반환(-)

	UFUNCTION()
		float GetGameSpeed();//게임속도반환(+)

	UFUNCTION()
		int32 GetGameLevel();//게임레벨반환

	void ReduceGameSpeed();//게임 속도 줄이기

protected:
	UPROPERTY()//게임스피드
		float gameSpeed;

	UPROPERTY()//게임레벨
		int32 gameLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)//속도증가를 위한 코인필요수
		int32 numCoinsForSpeedIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)//게임스피드 증가량
		float gameSpeedIncrease;



public:
	UPROPERTY()//런타임
		float RunTime;

	UFUNCTION()//틱
		virtual void Tick(float DeltaTime) override;

	UFUNCTION()//게임오버여부
		bool GetGameOver();

	UFUNCTION()//게임오버
		void GameOver();

	UFUNCTION()//일시정지
		void SetGamePaused(bool GamePaused);

	UPROPERTY()//게임오버 변수
		bool bGameOver;

	UPROPERTY()//게임 런타임 카운트
		bool StartGameOverCount;

	UPROPERTY()//게임오버후 지연시간
		float TimeTillGameOver;

	UPROPERTY()//게임오버후 타이머
		float GameOverTimer;

	float GetRunTime();//게임 런타임 반환
};



