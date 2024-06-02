// Fill out your copyright notice in the Description page of Project Settings.


#include "R1PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "System/R1AssetManager.h"
#include "Data/R1InputData.h"
#include "R1GameplayTags.h"

AR1PlayerController::AR1PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
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

		auto Action1 = InputData->FindInputActionByTag(R1GameplayTags::Input_Action_Move);
		EnhancedInputComponent->BindAction(Action1, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);

		auto Action2 = InputData->FindInputActionByTag(R1GameplayTags::Input_Action_Turn);
		EnhancedInputComponent->BindAction(Action2, ETriggerEvent::Triggered, this, &ThisClass::Input_Turn);
	}


	// if (auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	// {
	// 	EnhancedInputComponent->BindAction(TestAction, ETriggerEvent::Triggered, this, &ThisClass::Input_Test);
	// 	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	// 	EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ThisClass::Input_Turn);
	// }
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
		//FVector Direction = FVector::ForwardVector * MovementVector.X;
		// Direction print out the value of the direction
		//GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Cyan, FString::Printf(TEXT("X Direction: %s"), *Direction.ToString()));
		//GetPawn()->AddActorWorldOffset(Direction * 50.f);

		// Player Controller 의 회전값을 기준으로 동작하게 만듬
		FRotator Rotator = GetControlRotation();
		FVector Direction = UKismetMathLibrary::GetForwardVector(FRotator(0, Rotator.Yaw, 0));
		GetPawn()->AddMovementInput(Direction, MovementVector.X);
	}

	if (MovementVector.Y != 0)
	{
		//FVector Direction = FVector::RightVector * MovementVector.Y;
		// Direction print out the value of the direction
		//GEngine->AddOnScreenDebugMessage(0 , 3.0f , FColor::Cyan , FString::Printf(TEXT("Y Direction: %s") , *Direction.ToString()));
		//GetPawn()->AddActorWorldOffset(Direction * 50.f);

		FRotator Rotator = GetControlRotation();
		FVector Direction = UKismetMathLibrary::GetRightVector(FRotator(0, Rotator.Yaw, 0));
		GetPawn()->AddMovementInput(Direction, MovementVector.Y);
	}
}

void AR1PlayerController::Input_Turn(const FInputActionValue& InputValue)
{
	float Val = InputValue.Get<float>();
	AddYawInput(Val);
}
