// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/R1UserWidget.h"
#include "R1InventorySlotsWidget.generated.h"

class UCanvasPanel;
class UR1ItemInstance;
class UR1InventoryEntryWidget;
class UR1InventorySlotWidget;
class UUniformGridPanel;

/**
 * 
 */
UCLASS()
class R1_API UR1InventorySlotsWidget : public UR1UserWidget
{
	GENERATED_BODY()

public:
	UR1InventorySlotsWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativeConstruct() override;
	void OnInventoryEntryChanged(const FIntPoint& InItemSlotPos, TObjectPtr<UR1ItemInstance> Item);

	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	void FinishDrag();

protected:
	UPROPERTY()
	TSubclassOf<UR1InventorySlotWidget> SlotWidgetClass;

	UPROPERTY()
	TArray<TObjectPtr<UR1InventorySlotWidget>> SlotWidgets;

	UPROPERTY()
	TSubclassOf<UR1InventoryEntryWidget> EntryWidgetClass;

	UPROPERTY()
	TArray<TObjectPtr<UR1InventoryEntryWidget>> EntryWidgets;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUniformGridPanel> GridPanel_Slots;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel_Entries;

private:
	FIntPoint PrevDragOverSlotPos = FIntPoint(-1, -1);
	const int X_COUNT = 10;
	const int Y_COUNT = 5;
};
