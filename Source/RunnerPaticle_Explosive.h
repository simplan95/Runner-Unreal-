// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runner.h"
#include "RunnerObject.h"
#include "RunnerPaticle_Explosive.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API ARunnerPaticle_Explosive : public ARunnerObject
{
	GENERATED_BODY()
public:
	ARunnerPaticle_Explosive();

	UPROPERTY()
		UParticleSystemComponent* PaticleSystem;
};
