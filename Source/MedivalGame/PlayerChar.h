// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "PlayerChar.generated.h"

/**
 * 
 */
UCLASS()
class MEDIVALGAME_API APlayerChar : public ACharacterBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	APlayerChar();
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bDodegeForward = false;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bDodgeBackward =  false;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bDodgeLeft = false;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bDodgeRight = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bTurnRight = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bTurnLeft = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void Turn(float AxisValue);

	void DodgeFwd();
	void DodgeBwd();
	void DodgeLeft();
	void DodgeRight();
	
};
