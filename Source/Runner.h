// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(Runner, Log, All);//�α� ī�װ� ����

//���ӿ��带 �ٷ�� ���ø� �Լ� ���Ӹ�带 ��� �� ���Ӹ�带 �Լ��� �����ϴ� ������ ���ø� Ÿ������ ����ȯ 
template<typename T>
T* GetCustomGameMode(UWorld* worldContext)
{
	return Cast<T>(worldContext->GetAuthGameMode());
}