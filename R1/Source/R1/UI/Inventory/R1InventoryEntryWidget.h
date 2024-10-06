// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/R1UserWidget.h"
#include "R1InventoryEntryWidget.generated.h"

class UR1ItemInstance;
class UR1InventorySlotsWidget;
class USizeBox;
class UTextBlock;
class UImage;
class UR1ItemDragWidget;

/**
 * 
 */
UCLASS()
class R1_API UR1InventoryEntryWidget : public UR1UserWidget
{
	GENERATED_BODY()

public:
	UR1InventoryEntryWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	void Init(UR1InventorySlotsWidget* InSlotsWidget, UR1ItemInstance* InItemInstance, int32 InItemCount);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void RefreshWidgetOpacity(bool bClearlyVisible);
	void RefreshItemCount(int32 NewItemCount);

private:
	FIntPoint CachedFromSlotPos = FIntPoint::ZeroValue;
	FVector2D CachedDeltaWidgetPos = FVector2D::ZeroVector;
	int32 ItemCount = 0;

protected:
	UPROPERTY()
	TObjectPtr<UR1InventorySlotsWidget> SlotsWidget;

	UPROPERTY()
	TObjectPtr<UR1ItemInstance> ItemInstance;

	UPROPERTY()
	TSubclassOf<UR1ItemDragWidget> DragWidgetClass;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Count;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Hover;
};
