// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "RunnerGameMode.h"

//캐릭터 애님인스턴스
URunnerAnimInstance::URunnerAnimInstance()
{
	//캐릭터 속도를 500으로지정
	MySpeed = 500.0f;
}

void URunnerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}