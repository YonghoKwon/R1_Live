// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/R1InventoryEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "UI/Inventory/R1InventorySlotsWidget.h"
#include "Item/R1ItemInstance.h"
#include "UI/Item/Drag/R1DragDropOperation.h"
#include "UI/Item/Drag/R1ItemDragWidget.h"

UR1InventoryEntryWidget::UR1InventoryEntryWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UR1ItemDragWidget> FindDragWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Item/WBP_ItemDrag.WBP_ItemDrag_C'"));
	if (FindDragWidgetClass.Succeeded())
	{
		DragWidgetClass = FindDragWidgetClass.Class;
	}
}

void UR1InventoryEntryWidget::Init(UR1InventorySlotsWidget* InSlotsWidget, UR1ItemInstance* InItemInstance,
	int32 InItemCount)
{
	SlotsWidget = InSlotsWidget;
	ItemInstance = InItemInstance;
	ItemCount = InItemCount;
}

void UR1InventoryEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Text_Count->SetText(FText::GetEmpty());
}

void UR1InventoryEntryWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry , InMouseEvent);

	Image_Hover->SetRenderOpacity(1.f);
}

void UR1InventoryEntryWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	Image_Hover->SetRenderOpacity(0.f);
}

FReply UR1InventoryEntryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry , InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		// Do something
		Reply.DetectDrag(TakeWidget() , EKeys::LeftMouseButton);
	}

	const FIntPoint UnitInventorySlotSize = FIntPoint(50, 50);

	FVector2D MouseWidgetPos = SlotsWidget->GetCachedGeometry().AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	FVector2D ItemWidgetPos = SlotsWidget->GetCachedGeometry().AbsoluteToLocal(InGeometry.LocalToAbsolute(UnitInventorySlotSize / 2.f));
	FIntPoint ItemSlotPos = FIntPoint(ItemWidgetPos.X / UnitInventorySlotSize.X, ItemWidgetPos.Y / UnitInventorySlotSize.Y);

	CachedFromSlotPos = ItemSlotPos;
	CachedDeltaWidgetPos = MouseWidgetPos - ItemWidgetPos;

	return Reply;
}

void UR1InventoryEntryWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	OUT UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry , InMouseEvent , OutOperation);

	UR1ItemDragWidget* DragWidget = CreateWidget<UR1ItemDragWidget>(GetOwningPlayer(), DragWidgetClass);
	FVector2D EntityMidgetSize = FVector2D(1 * 50, 1 * 50);
	DragWidget->Init(EntityMidgetSize , nullptr , ItemCount);

	UR1DragDropOperation* DragDrop = NewObject<UR1DragDropOperation>();
	DragDrop->DefaultDragVisual = DragWidget;
	DragDrop->Pivot = EDragPivot::MouseDown;
	DragDrop->FromItemSlotPos = CachedFromSlotPos;
	DragDrop->ItemInstance = ItemInstance;
	DragDrop->DeltaWidgetPos = CachedDeltaWidgetPos;

	OutOperation = DragDrop;
}

void UR1InventoryEntryWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent , InOperation);

	RefreshWidgetOpacity(true);
}

void UR1InventoryEntryWidget::RefreshWidgetOpacity(bool bClearlyVisible)
{
	SetRenderOpacity(bClearlyVisible ? 1.f : 0.5f);
}

void UR1InventoryEntryWidget::RefreshItemCount(int32 NewItemCount)
{
	ItemCount = NewItemCount;
	Text_Count->SetText((ItemCount >= 2) ? FText::AsNumber(ItemCount) : FText::GetEmpty());
}
