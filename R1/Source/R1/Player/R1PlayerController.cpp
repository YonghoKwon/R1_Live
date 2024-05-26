// Fill out your copyright notice in the Description page of Project Settings.


#include "R1PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMaterialLibrary.h"

AR1PlayerController::AR1PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AR1PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext , 0);

	}
}

void AR1PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(TestAction, ETriggerEvent::Triggered, this, &ThisClass::Input_Test);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ThisClass::Input_Turn);
	}
}

void AR1PlayerController::Input_Test(const FInputActionValue& InputValue)
{
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Cyan, TEXT("Test Action Triggered"));
}

void AR1PlayerController::Input_Move(const FInputActionValue& InputValue)
{
	FVector2D MovementVector = InputValue.Get<FVector2D>();
	// MovementVector print out the value of the vector
	GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Cyan, FString::Printf(TEXT("MovementVector: %s"), *MovementVector.ToString()));

	if (MovementVector.X != 0)
	{
		FVector Direction = FVector::ForwardVector * MovementVector.X;
		// Direction print out the value of the direction
		//GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Cyan, FString::Printf(TEXT("X Direction: %s"), *Direction.ToString()));
		GetPawn()->AddActorWorldOffset(Direction * 50.f);
	}

	if (MovementVector.Y != 0)
	{
		FVector Direction = FVector::RightVector * MovementVector.Y;
		// Direction print out the value of the direction
		//GEngine->AddOnScreenDebugMessage(0 , 3.0f , FColor::Cyan , FString::Printf(TEXT("Y Direction: %s") , *Direction.ToString()));
		GetPawn()->AddActorWorldOffset(Direction * 50.f);
	}
}

void AR1PlayerController::Input_Turn(const FInputActionValue& InputValue)
{
	float Val = InputValue.Get<float>();
	AddYawInput(Val);
}
