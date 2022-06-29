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

	//틱마다 호출 되는 가상함수
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float MySpeed; //애님그래프에서 속도 값을 사용할 수 있도록 설정

};
