// Fill out your copyright notice in the Description page of Project Settings.


#include "R1Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
/**
 * 
 */
AR1Pawn::AR1Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	RootComponent = CapsuleComponent;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CapsuleComponent);
	// set skeletal mesh "/Script/Engine.SkeletalMesh'/Game/_Art/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'";
	// set skeletal mesh "/Script/Engine.SkeletalMesh'/Game/_Art/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'";
	// 메시를 로드합니다.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshObj(TEXT("/Game/_Art/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshObj(TEXT("/Game/_Art/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin"));

	// 메시가 성공적으로 로드되었는지 확인합니다.
	if (SkeletalMeshObj.Succeeded())
	{
		// 스켈레탈 메시 컴포넌트에 메시를 설정합니다.
		Mesh->SetSkeletalMesh(SkeletalMeshObj.Object);
	}
	Mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(CapsuleComponent);
	SpringArm->TargetArmLength = 700.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void AR1Pawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AR1Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AR1Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

