// Fill out your copyright notice in the Description page of Project Settings.


#include "System/R1AssetManager.h"
#include "R1LogChannels.h"

UR1AssetManager::UR1AssetManager() : Super()
{
}

UR1AssetManager& UR1AssetManager::Get()
{
	UE_LOG(LogR1, Log, TEXT("UR1AssetManager::Get() Start"));

	if (UR1AssetManager* Singleton = Cast<UR1AssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogR1, Fatal, TEXT("Can't find UR1AssetManger class"));

	return *NewObject<UR1AssetManager>();
}
