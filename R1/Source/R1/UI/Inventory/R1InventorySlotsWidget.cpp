// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/R1InventorySlotsWidget.h"

#include "R1InventoryEntryWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/UniformGridPanel.h"
#include "Item/R1InventorySubsystem.h"
#include "UI/Inventory/R1InventorySlotWidget.h"
#include "UI/Inventory/R1InventoryEntryWidget.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"
#include "UI/Item/Drag/R1DragDropOperation.h"
#include "R1Define.h"

UR1InventorySlotsWidget::UR1InventorySlotsWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UR1InventorySlotWidget> FindSlotWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Item/Inventory/WBP_InventorySlot.WBP_InventorySlot_C'"));
	if (FindSlotWidgetClass.Succeeded())
	{
		SlotWidgetClass = FindSlotWidgetClass.Class;
	}

	ConstructorHelpers::FClassFinder<UR1InventoryEntryWidget> FindEntryWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Item/Inventory/WBP_InventoryEntry.WBP_InventoryEntry_C'"));
	if (FindEntryWidgetClass.Succeeded())
	{
		EntryWidgetClass = FindEntryWidgetClass.Class;
	}

}

void UR1InventorySlotsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SlotWidgets.SetNum(X_COUNT * Y_COUNT);

	for (int32 y = 0; y < Y_COUNT; y++)
	{
		for (int32 x = 0; x < X_COUNT; x++)
		{
			int32 index = y * X_COUNT + x;

			UR1InventorySlotWidget* SlotWidget = CreateWidget<UR1InventorySlotWidget>(GetOwningPlayer(), SlotWidgetClass);
			SlotWidgets[index] = SlotWidget;
			GridPanel_Slots->AddChildToUniformGrid(SlotWidget , y , x);
		}
	}

	EntryWidgets.SetNum(X_COUNT * Y_COUNT);

	UR1InventorySubsystem* Inventory = Cast<UR1InventorySubsystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(this, UR1InventorySubsystem::StaticClass()));

	const TArray<TObjectPtr<UR1ItemInstance>>& Items = Inventory->GetItems();

	for (int32 i = 0; i < Items.Num(); i++)
	{
		const TObjectPtr<UR1ItemInstance>& Item = Items[i];
		FIntPoint ItemSlotPos = FIntPoint(i % X_COUNT, i / X_COUNT);
		OnInventoryEntryChanged(ItemSlotPos, Item);
	}
}

void UR1InventorySlotsWidget::OnInventoryEntryChanged(const FIntPoint& InItemSlotPos, TObjectPtr<UR1ItemInstance> Item)
{
	int32 SlotIndex = InItemSlotPos.Y * X_COUNT + InItemSlotPos.X;

	if (UR1InventoryEntryWidget* EntryWidget = EntryWidgets[SlotIndex])
	{
		if (Item == nullptr)
		{
			CanvasPanel_Entries->RemoveChild(EntryWidget);
			EntryWidgets[SlotIndex] = nullptr;
		}
	}
	else
	{
		EntryWidget = CreateWidget<UR1InventoryEntryWidget>(GetOwningPlayer(), EntryWidgetClass);
		EntryWidgets[SlotIndex] = EntryWidget;

		UCanvasPanelSlot* CanvasPanelSlot = CanvasPanel_Entries->AddChildToCanvas(EntryWidget);
		CanvasPanelSlot->SetAutoSize(true);
		CanvasPanelSlot->SetPosition(FVector2D(InItemSlotPos.X * 50, InItemSlotPos.Y * 50));

		// TODO
		EntryWidget->Init(this , Item , 1);
	}
}

bool UR1InventorySlotsWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry , InDragDropEvent , InOperation);

	UR1DragDropOperation* DragDrop = Cast<UR1DragDropOperation>(InOperation);
	check(DragDrop);

	FVector2D MouseWidgetPos = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	FVector2D ToWidgetPos = MouseWidgetPos - DragDrop->DeltaWidgetPos;
	FIntPoint ToSlotPos = FIntPoint(ToWidgetPos.X / Item::UnitInventorySlotSize.X, ToWidgetPos.Y / Item::UnitInventorySlotSize.Y);

	if (PrevDragOverSlotPos == ToSlotPos)
		return true;

	PrevDragOverSlotPos = ToSlotPos;

	// TODO

	return false;
}

void UR1InventorySlotsWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent , InOperation);
	FinishDrag();
}

bool UR1InventorySlotsWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry , InDragDropEvent , InOperation);
	FinishDrag();

	UR1DragDropOperation* DragDrop = Cast<UR1DragDropOperation>(InOperation);
	check(DragDrop);

	FVector2D MouseWidgetPos = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	FVector2D ToWidgetPos = MouseWidgetPos - DragDrop->DeltaWidgetPos;
	FIntPoint ToSlotPos = FIntPoint(ToWidgetPos.X / Item::UnitInventorySlotSize.X, ToWidgetPos.Y / Item::UnitInventorySlotSize.Y);

	// TODO
	if (DragDrop->FromItemSlotPos != ToSlotPos)
	{
		OnInventoryEntryChanged(DragDrop->FromItemSlotPos, nullptr);
		OnInventoryEntryChanged(ToSlotPos, DragDrop->ItemInstance);
	}

	return false;
}

void UR1InventorySlotsWidget::FinishDrag()
{
	PrevDragOverSlotPos = FIntPoint(-1, -1);
}
