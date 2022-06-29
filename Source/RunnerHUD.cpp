// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerHUD.h"
#include "Engine/Font.h"//��Ʈ
#include "Engine/Canvas.h"//ĵ����
#include "RunnerCharacter.h"//����ĳ����
#include "RunnerGameMode.h"//���� ���Ӹ��

//���� �÷��� �� �÷��̾�HUD
ARunnerHUD::ARunnerHUD()
{
	static ConstructorHelpers::FObjectFinder<UFont> OurHudFont(TEXT("/Game/AssetForRunner/Font/AsiaM_Font.AsiaM_Font"));
	HUDFont = OurHudFont.Object;
}

void ARunnerHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);//ĵ�������� ȭ��ũ�⸦ ������

	ARunnerCharacter* RunnerCharacter = Cast<ARunnerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	ARunnerGameMode* RunnerGameMode = GetCustomGameMode<ARunnerGameMode>(GetWorld());

	//�ؽ�Ʈ ����
	FString HUDString = FString::Printf(TEXT("Score : %d  Level : %d  RunTime : %.4f"), RunnerCharacter->GetScore(), RunnerGameMode->GetGameLevel(),RunnerGameMode->GetRunTime());

	DrawText(HUDString, FColor::Red, 50, 50, HUDFont);// 50 50 ��ǥ�� HUD ����

	if (RunnerGameMode->GetGameOver())
	{
		FVector2D GameOverSize;
		GetTextSize(TEXT("GAME OVER!!! Press R to Restart"), GameOverSize.X, GameOverSize.Y, HUDFont);//�ؽ�Ʈ ������ ���ϱ�

		DrawText(TEXT("GAME OVER!!! Press R to Restart"), FColor::Red, ((ScreenDimensions.X - GameOverSize.X) / 2.0f), ((ScreenDimensions.Y - GameOverSize.Y) / 2.0f), HUDFont);
		RunnerGameMode->SetGamePaused(true);
	}
}