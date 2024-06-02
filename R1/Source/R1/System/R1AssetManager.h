// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "R1LogChannels.h"
#include "CoreMinimal.h"
#include "Data/R1AssetData.h"
#include "Engine/AssetManager.h"
#include "R1AssetManager.generated.h"

class UR1AssetData;
/**
 * 
 */
UCLASS()
class R1_API UR1AssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UR1AssetManager();

	static UR1AssetManager& Get();

public:
	static void Initialize();

	template<typename AssetType>
	static AssetType* GetAssetByName(const FName& AssetName);

	static void LoadSyncByPath(const FSoftObjectPath& AssetPath);
	static void LoadSyncByName(const FName& AssetName);
	static void LoadSyncByLabel(const FName& Label);

	static void ReleaseByPath(const FSoftObjectPath& AssetPath);
	static void ReleaseByName(const FName& AssetName);
	static void ReleaseByLabel(const FName& Label);
	static void ReleaseAll();

private:
	void LoadPreloadAssets();
	void AddLoadedAsset(const FName& AssetName, const UObject* Asset);

	// TODO : 로드한 에셋들을 가지고 있는...
private:
	UPROPERTY()
	TObjectPtr<UR1AssetData> LoadedAssetData;

	UPROPERTY()
	TMap<FName, TObjectPtr<const UObject>> NameToLoadedAsset;

	// FCriticalSection LoadedAssetsCritical;
};

template <typename AssetType>
AssetType* UR1AssetManager::GetAssetByName(const FName& AssetName)
{
	UR1AssetData* AssetData = Get().LoadedAssetData;
	check(AssetData);

	AssetType* LoadedAsset = nullptr;
	const FSoftObjectPath& AssetPath = AssetData->GetAssetPathByName(AssetName);
	if (AssetPath.IsValid())
	{
		LoadedAsset = Cast<AssetType>(AssetPath.ResolveObject());
		if (LoadedAsset == nullptr)
		{
			UE_LOG(LogR1, Warning, TEXT("Attempted sync loading because asset hadn't loaded yet [%s]."), *AssetPath.ToString());
			LoadedAsset = Cast<AssetType>(AssetPath.TryLoad());
		}
	}

	return LoadedAsset;
}
