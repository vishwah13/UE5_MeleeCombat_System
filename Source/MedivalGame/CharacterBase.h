// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>
#include "CharacterBase.generated.h"

UCLASS()
class MEDIVALGAME_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilitySystem, meta = (AllowPrivateAccess = "true"))
	class UGASAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY()
	class UGASAttributeSet* Attribute;

public:
	// Sets default values for this character's properties
	ACharacterBase();
	
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UChildActorComponent* SwordActor;*/
	
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void InitializeAttributes();
	virtual void GiveAbility();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray < TSubclassOf<class UGASGameplayAbility>> DefaultAbility;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FightCollision")
	class USphereComponent* leftHandSphereCollisionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FightCollision")
	class USphereComponent* rightHandSphereCollisionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FightCollision")
	class USphereComponent* leftLegSphereCollisionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FightCollision")
	class USphereComponent* rightLegSphereCollisionComponent;

	UPROPERTY(EditDefaultsOnly,Category = "Enemy")
	TSubclassOf< AActor > TargetActorClass;
	TArray< AActor* > OutActors;

	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	TArray<AActor*> ignoreActors;
	UPROPERTY(EditAnywhere, Category = "Enemy")
	float SphereRadius = 750.0f;
	UPROPERTY(EditAnywhere, Category = "Enemy")
	float EnemyDistance = 750.0f;

	UPROPERTY()
	class AEnemy* ClosestEnemy;

	virtual void BeginPlay() override;

	void FindClosestTarget();
	void RotateToTarget(AEnemy* target, float deltaTime);
	bool bIsTargetLocked = false;
	bool bIsAttacking = false;

public:	
	UFUNCTION(BlueprintCallable)
	void Attacking();

	UFUNCTION(BlueprintCallable)
	void Attack(USphereComponent* sphereCollision);
	void Damage(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
