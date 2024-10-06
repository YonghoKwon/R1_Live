// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/R1UserWidget.h"
#include "R1ItemDragWidget.generated.h"

class UTexture2D;
class UTextBlock;
class UImage;
class USizeBox;
/**
 * 
 */
UCLASS()
class R1_API UR1ItemDragWidget : public UR1UserWidget
{
	GENERATED_BODY()

public:
	UR1ItemDragWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	void Init(const FVector2D& InWidgetSize, UTexture2D* InItemIcon, int32 InItemCount);


protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Count;
};
