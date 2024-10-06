// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/R1UserWidget.h"
#include "R1InventorySlotWidget.generated.h"

class UImage;
class USizeBox;

/**
 * 
 */
UCLASS()
class R1_API UR1InventorySlotWidget : public UR1UserWidget
{
	GENERATED_BODY()

public:
	UR1InventorySlotWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Slot;
};
