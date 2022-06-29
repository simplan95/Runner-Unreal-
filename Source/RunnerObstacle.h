// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runner.h"
#include "RunnerObject.h"
#include "RunnerObstacle.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)//�������Ʈ Ÿ������ ����
class RUNNER_API ARunnerObstacle : public ARunnerObject
{
	GENERATED_BODY()

	ARunnerObstacle();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)//�ش� �޽ô� �������Ʈ���� ����
		UStaticMeshComponent* Mesh;
		
};
