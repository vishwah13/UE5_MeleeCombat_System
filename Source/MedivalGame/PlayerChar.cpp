// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APlayerChar::APlayerChar()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCamera->SetupAttachment(CameraBoom);
}

void APlayerChar::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerChar::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerChar::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerChar::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayerChar::Turn);

	//Input for Dodging
	PlayerInputComponent->BindAction(TEXT("DodgeForward"), EInputEvent::IE_Pressed, this, &APlayerChar::DodgeFwd);
	PlayerInputComponent->BindAction(TEXT("DodgeBackward"), EInputEvent::IE_Pressed, this,  &APlayerChar::DodgeBwd);
	PlayerInputComponent->BindAction(TEXT("DodgeLeft"), EInputEvent::IE_Pressed, this,  &APlayerChar::DodgeLeft);
	PlayerInputComponent->BindAction(TEXT("DodgeRight"), EInputEvent::IE_Pressed, this,  &APlayerChar::DodgeRight);
	
	PlayerInputComponent->BindAction(TEXT("DodgeForward"), EInputEvent::IE_Released, this, &APlayerChar::DodgeFwd);
	PlayerInputComponent->BindAction(TEXT("DodgeBackward"), EInputEvent::IE_Released, this, &APlayerChar::DodgeBwd);
	PlayerInputComponent->BindAction(TEXT("DodgeLeft"), EInputEvent::IE_Released, this, &APlayerChar::DodgeLeft);
	PlayerInputComponent->BindAction(TEXT("DodgeRight"), EInputEvent::IE_Released, this, &APlayerChar::DodgeRight);
}

void APlayerChar::MoveForward(float AxisValue)
{
	bool IsAnyMontagePlaying = GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();
	
	if (!IsAnyMontagePlaying)
	{
		AddMovementInput(GetActorForwardVector() * AxisValue);
	}
	
	
}

void APlayerChar::MoveRight(float AxisValue)
{
	bool IsAnyMontagePlaying = GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();
	if (!IsAnyMontagePlaying)
	{
		AddMovementInput(GetActorRightVector() * AxisValue);
	}
		
	
}

void APlayerChar::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void APlayerChar::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);

	if (GetVelocity().Length() == 0)
	{
		if (AxisValue > 0.3f)
		{
			bTurnRight = true;
			bTurnLeft = false;
		}
		else
		{
			bTurnRight = false;
		}
		if (AxisValue < -0.3f)
		{
			bTurnRight = false;
			bTurnLeft = true;
		}
		else
		{
			bTurnLeft = false;
		}
	}
	else
	{
		bTurnRight = false;
		bTurnLeft = false;
	}
}

void APlayerChar::DodgeFwd()
{
	bDodegeForward = !bDodegeForward;
}

void APlayerChar::DodgeBwd()
{
	bDodgeBackward = !bDodgeBackward;
}

void APlayerChar::DodgeLeft()
{
	bDodgeLeft = !bDodgeLeft;
}

void APlayerChar::DodgeRight()
{
	bDodgeRight = !bDodgeRight;
}

