// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/R1Player.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/R1PlayerController.h"

AR1Player::AR1Player()
{
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;  // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(GetCapsuleComponent());

	SpringArm->TargetArmLength = 800.f;
	SpringArm->SetRelativeRotation(FRotator(-60 , 0 , 0));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;  // Camera does not rotate relative to arm

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void AR1Player::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);

	// Delegate
	RookissDelegate.BindUObject(this, &ThisClass::TestFunc);

	// 실행
	RookissDelegate.Execute();

	// 해제
	RookissDelegate.Unbind();

	auto h1 = RookissMulticastDelegate.AddUObject(this, &ThisClass::TestFunc);
	RookissMulticastDelegate.AddUObject(this, &ThisClass::TestFunc);

	// 실행
	RookissMulticastDelegate.Broadcast();

	RookissMulticastDelegate.Remove(h1);
	// RookissMulticastDelegate.RemoveAll();


}

void AR1Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AR1Player::HandleGameplayEvent(FGameplayTag EventTag)
{
	Super::HandleGameplayEvent(EventTag);

	// TODO :
	AR1PlayerController* PC = Cast<AR1PlayerController>(GetController());
	if (PC)
	{
		PC->HandleGameplayEvent(EventTag);
	}
}

void AR1Player::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("OnBeginOverlap"));
}
