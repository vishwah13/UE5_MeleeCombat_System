// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "AbilitySystemComponent.h"
#include "GASAbilitySystemComponent.h"
#include "GASGameplayAbility.h"
#include "GASAttributeSet.h"
#include "MedivalGame.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"


// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	check(GetMesh());
	SwordActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("Sword"));
	SwordActor->SetMobility(EComponentMobility::Movable);
	SwordActor->SetupAttachment(GetMesh(), TEXT("SwordSocket"));	
	
	
	
	//GamePlay Ability System
	AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	
	Attribute = CreateDefaultSubobject<UGASAttributeSet>(TEXT("Attribute"));

	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	ignoreActors.Init(this, 1);
}

class UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACharacterBase::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect,1, EffectContext);
		
		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ACharacterBase::GiveAbility()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UGASGameplayAbility>& StartupAbility : DefaultAbility)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
				StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}
	}
}

void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//server GAS Init
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	InitializeAttributes();
	GiveAbility();
}

void ACharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeAttributes();
	
	//binding Input to GAS
	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds(
			"Confirm", "Cancel", "EGASAbilityInputID",
			static_cast<int32>(EGASAbilityInputID::Confirm),
			static_cast<int32>(EGASAbilityInputID::Cancel));

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::FindClosestTarget()
{

	if (!bIsTargetLocked)
	{
		ClosestEnemy = nullptr;
		OutActors.Empty();

		DrawDebugSphere(GetWorld(), GetActorLocation(), SphereRadius, 12, FColor::Red, false, 5.f);

		if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), SphereRadius, traceObjectTypes, TargetActorClass, ignoreActors, OutActors))
		{
			AActor* enemy =	UGameplayStatics::FindNearestActor(GetActorLocation(), OutActors, EnemyDistance);
			ClosestEnemy = Cast<AEnemy>(enemy);
			//UE_LOG(LogTemp, Warning, TEXT("enemy is %s"), *ClosestEnemy->GetName());
			ClosestEnemy->DecaleComponent->SetVisibility(true);
			bIsTargetLocked = true;
			
		}
	}
	else if(bIsTargetLocked)
	{
		bIsTargetLocked = false;
		ClosestEnemy->DecaleComponent->SetVisibility(false);
		ClosestEnemy = nullptr;
	}
	
}


void ACharacterBase::RotateToTarget(AEnemy* target, float deltaTime)
{
	if (target != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Actor %s is from Distance to Player is %f"), *target->GetName(), GetDistanceTo(target));
		FVector start = GetActorLocation();
		FVector End = target->GetActorLocation();

		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(start, End);
		FRotator target = FRotator(GetActorRotation().Pitch, LookAtRotation.Yaw, GetActorRotation().Roll);
		FRotator RotateTo = UKismetMathLibrary::RInterpTo(GetActorRotation(), target, deltaTime, 0.0f);
		GetController()->SetControlRotation(RotateTo);
	}
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsTargetLocked)
	{
		if (ClosestEnemy == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("ClosestEnemy is NULL."))
		}
		
		RotateToTarget(ClosestEnemy, DeltaTime);
	}
	
	
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("LockOn"), EInputEvent::IE_Pressed, this, &ACharacterBase::FindClosestTarget);

	//binding Input to GAS
	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds(
			"Confirm", "Cancel", "EGASAbilityInputID",
			static_cast<int32>(EGASAbilityInputID::Confirm),
			static_cast<int32>(EGASAbilityInputID::Cancel));

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}

}



