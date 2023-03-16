// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemBlueprintLibrary.h"

// Sets default values
ASword::ASword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);
	
	SwordMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetupAttachment(RootComponent);
		

}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerActor = GetParentActor();
}

// Called every frame
void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSwordLineTrace)
	{
		SwordLineTrace();
	}

}

void ASword::StartSwordTrigger()
{
	//UE_LOG(LogTemp, Warning, TEXT("Started"));
	//GetWorldTimerManager().SetTimer(SwordLineTraceTimeHandler, this, &ASword::SwordLineTrace, 0.01f, true, 0);
	bSwordLineTrace = true;
}

void ASword::SwordLineTrace()
{
	//UE_LOG(LogTemp, Warning, TEXT("Processing"));
	Start = SwordMesh->GetSocketLocation(TEXT("Start"));
	End = SwordMesh->GetSocketLocation(TEXT("End"));
	FHitResult HitResult;

	bool bHitSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel1);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 3.0f);
	

	FGameplayEventData Payload;
	FGameplayTag Tag;
	Tag = FGameplayTag::RequestGameplayTag(TEXT("Weapon.Hit"), true);


	if (HitResult.GetActor() != OwnerActor && HitResult.bBlockingHit)
	{
		Payload.Instigator = OwnerActor->GetInstigator();
		Payload.Target = HitResult.GetActor();

		Payload.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(HitResult.GetActor());

		//UE_LOG(LogTemp, Warning, TEXT("Actor name is : %s"),*HitResult.GetActor()->GetName());
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor->GetInstigator(), Tag, Payload);
	}
}

void ASword::StopSwordTrigger()
{
	//UE_LOG(LogTemp, Warning, TEXT("Stoped"));
	//GetWorldTimerManager().ClearTimer(SwordLineTraceTimeHandler);
	bSwordLineTrace = false;
}

