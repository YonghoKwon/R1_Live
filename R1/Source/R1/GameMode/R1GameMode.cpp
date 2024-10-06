// Fill out your copyright notice in the Description page of Project Settings.


#include "R1GameMode.h"

#include "Item/R1InventorySubsystem.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"

void AR1GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName , Options , ErrorMessage);

	UR1InventorySubsystem* Inventory = Cast<UR1InventorySubsystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(this, UR1InventorySubsystem::StaticClass()));
	if ( Inventory )
	{
		Inventory->AddDefaultItems();
	}
}
