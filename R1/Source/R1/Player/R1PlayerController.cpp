// Fill out your copyright notice in the Description page of Project Settings.


#include "R1PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "System/R1AssetManager.h"
#include "Data/R1InputData.h"
#include "R1GameplayTags.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Character/R1Player.h"

AR1PlayerController::AR1PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AR1PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	// {
	// 	Subsystem->AddMappingContext(InputMappingContext , 0);
	// }

	if (const UR1InputData* InputData = UR1AssetManager::GetAssetByName<UR1InputData>("InputData"))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputData->InputMappingContext, 0);
		}
	}
}

void AR1PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (const UR1InputData* InputData = UR1AssetManager::GetAssetByName<UR1InputData>("InputData"))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		auto Action1 = InputData->FindInputActionByTag(R1GameplayTags::Input_Action_SetDestination);

		// Setup mouse input events
		EnhancedInputComponent->BindAction(Action1, ETriggerEvent::Started, this, &ThisClass::OnInputStarted);
		EnhancedInputComponent->BindAction(Action1, ETriggerEvent::Triggered, this, &ThisClass::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(Action1, ETriggerEvent::Completed, this, &ThisClass::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(Action1, ETriggerEvent::Canceled, this, &ThisClass::OnSetDestinationReleased);
	}
}

void AR1PlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AR1PlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, OUT Hit);

	// If we hit a surface, cache the loaction
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AR1PlayerController::OnSetDestinationReleased()
{
	// If it was a shor press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}
