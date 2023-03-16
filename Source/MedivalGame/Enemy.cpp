// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/DecalComponent.h"

AEnemy::AEnemy()
{
	DecaleComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("TargetDecale"));
	DecaleComponent->SetupAttachment(GetMesh());
}
