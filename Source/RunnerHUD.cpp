// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerHUD.h"
#include "Engine/Font.h"//폰트
#include "Engine/Canvas.h"//캔버스
#include "RunnerCharacter.h"//러너캐릭터
#include "RunnerGameMode.h"//러너 게임모드

//게임 플레이 중 플레이어HUD
ARunnerHUD::ARunnerHUD()
{
	static ConstructorHelpers::FObjectFinder<UFont> OurHudFont(TEXT("/Game/AssetForRunner/Font/AsiaM_Font.AsiaM_Font"));
	HUDFont = OurHudFont.Object;
}

void ARunnerHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);//캔버스에서 화면크기를 가져옴

	ARunnerCharacter* RunnerCharacter = Cast<ARunnerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	ARunnerGameMode* RunnerGameMode = GetCustomGameMode<ARunnerGameMode>(GetWorld());

	//텍스트 정보
	FString HUDString = FString::Printf(TEXT("Score : %d  Level : %d  RunTime : %.4f"), RunnerCharacter->GetScore(), RunnerGameMode->GetGameLevel(),RunnerGameMode->GetRunTime());

	DrawText(HUDString, FColor::Red, 50, 50, HUDFont);// 50 50 좌표에 HUD 생성

	if (RunnerGameMode->GetGameOver())
	{
		FVector2D GameOverSize;
		GetTextSize(TEXT("GAME OVER!!! Press R to Restart"), GameOverSize.X, GameOverSize.Y, HUDFont);//텍스트 사이즈 구하기

		DrawText(TEXT("GAME OVER!!! Press R to Restart"), FColor::Red, ((ScreenDimensions.X - GameOverSize.X) / 2.0f), ((ScreenDimensions.Y - GameOverSize.Y) / 2.0f), HUDFont);
		RunnerGameMode->SetGamePaused(true);
	}
}