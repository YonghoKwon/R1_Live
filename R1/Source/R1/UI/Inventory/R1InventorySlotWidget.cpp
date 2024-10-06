// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/R1InventorySlotWidget.h"

#include "Components/SizeBox.h"

UR1InventorySlotWidget::UR1InventorySlotWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UR1InventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SizeBox_Root->SetWidthOverride(50);
	SizeBox_Root->SetHeightOverride(50);
}