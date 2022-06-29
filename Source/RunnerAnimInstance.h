// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runner.h"
#include "Animation/AnimInstance.h"
#include "RunnerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API URunnerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	URunnerAnimInstance();

	//ƽ���� ȣ�� �Ǵ� �����Լ�
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float MySpeed; //�ִԱ׷������� �ӵ� ���� ����� �� �ֵ��� ����

};
