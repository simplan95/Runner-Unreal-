// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "RunnerGameMode.h"

//ĳ���� �ִ��ν��Ͻ�
URunnerAnimInstance::URunnerAnimInstance()
{
	//ĳ���� �ӵ��� 500��������
	MySpeed = 500.0f;
}

void URunnerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}