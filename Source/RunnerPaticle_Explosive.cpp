// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerPaticle_Explosive.h"
#include "RunnerObstacle.h"

ARunnerPaticle_Explosive::ARunnerPaticle_Explosive()
{
	PaticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Explsion"));
	check(PaticleSystem);

	ConstructorHelpers::FObjectFinder<UParticleSystem>myPaticle(TEXT("/Game/M5VFXVOL2/Particles/Explosion/Fire_Exp_00.Fire_Exp_00"));
	if (myPaticle.Succeeded())
	{
		PaticleSystem->SetTemplate(myPaticle.Object);
		PaticleSystem->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	}
	PaticleSystem->AttachTo(RootComponent);
}