// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(Runner, Log, All);//로그 카테고리 정의

//게임월드를 다루는 템플릿 함수 게임모드를 얻고 이 게임모드를 함수가 제공하는 정보를 템플릿 타입으로 형변환 
template<typename T>
T* GetCustomGameMode(UWorld* worldContext)
{
	return Cast<T>(worldContext->GetAuthGameMode());
}