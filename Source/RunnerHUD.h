// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runner.h"
#include "GameFramework/HUD.h"
#include "RunnerHUD.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API ARunnerHUD : public AHUD
{
	GENERATED_BODY()

	ARunnerHUD();

	UPROPERTY()
		UFont* HUDFont;

	virtual void DrawHUD() override;
};
