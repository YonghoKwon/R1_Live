// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/R1Character.h"
#include "R1Define.h"
#include "Components/WidgetComponent.h"
#include "UI/R1HpBarWidget.h"

// Sets default values
AR1Character::AR1Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HpBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HpBarComponent->SetupAttachment(RootComponent);

	ConstructorHelpers::FClassFinder<UUserWidget> HealthBarWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/WBP_HpBar.WBP_HpBar_C'"));
	if (HealthBarWidget.Succeeded())
	{
		HpBarComponent->SetWidgetClass(HealthBarWidget.Class);
		HpBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
		HpBarComponent->SetRelativeLocation(FVector(0, 0, 130));
		HpBarComponent->SetDrawSize(FVector2D(80, 10));
		HpBarComponent->SetVisibility(true);
	}
}

// Called when the game starts or when spawned
void AR1Character::BeginPlay()
{
	Super::BeginPlay();
	RefreshHpBarRatio();
}

// Called every frame
void AR1Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AR1Character::HandleGameplayEvent(FGameplayTag EventTag)
{
}

void AR1Character::Highlight()
{
	bHighlighted = true;
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(250);
}

void AR1Character::Unhighlight()
{
	bHighlighted = false;
	GetMesh()->SetRenderCustomDepth(false);
}

void AR1Character::OnDamaged(int32 Damage, TObjectPtr<AR1Character> Attacker)
{
	Hp = FMath::Clamp(Hp - Damage, 0, MaxHp);
	if (Hp == 0)
	{
		OnDead(Attacker);
	}

	//D(FString::Printf(TEXT("%d"), Hp));
	RefreshHpBarRatio();
}

void AR1Character::OnDead(TObjectPtr<AR1Character> Attacker)
{
	if (CreatureState == ECreatureState::Dead)
	{
		return;
	}

	CreatureState = ECreatureState::Dead;
}

void AR1Character::RefreshHpBarRatio()
{
	if (HpBarComponent)
	{
		float Ratio = static_cast<float>(Hp) / MaxHp;
		UR1HpBarWidget* HpBar = Cast<UR1HpBarWidget>(HpBarComponent->GetUserWidgetObject());
		if (HpBar)
		{
			HpBar->SetHpRatio(Ratio);
		}
	}
}

