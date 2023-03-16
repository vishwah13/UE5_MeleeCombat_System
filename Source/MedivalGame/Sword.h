// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sword.generated.h"

UCLASS()
class MEDIVALGAME_API ASword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartSwordTrigger();

	bool bSwordLineTrace = false;

	UFUNCTION(BlueprintCallable)
	void StopSwordTrigger();

	void SwordLineTrace();

	FTimerHandle SwordLineTraceTimeHandler;	

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* SwordMesh;
	
	FVector Start;
	FVector End;
	
	AActor* OwnerActor;
};
