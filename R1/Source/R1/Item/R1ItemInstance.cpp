// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/R1ItemInstance.h"

UR1ItemInstance::UR1ItemInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UR1ItemInstance::Init(int32 InItemID)
{
	if (InItemID <= 0)
		return;

	ItemId = InItemID;
	Rarity = EItemRarity::Common;
}
