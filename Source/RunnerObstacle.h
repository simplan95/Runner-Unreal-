// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runner.h"
#include "RunnerObject.h"
#include "RunnerObstacle.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)//블루프린트 타입으로 설정
class RUNNER_API ARunnerObstacle : public ARunnerObject
{
	GENERATED_BODY()

	ARunnerObstacle();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)//해당 메시는 블루프린트에서 구현
		UStaticMeshComponent* Mesh;
		
};
