// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class MEDIVALGAME_API AEnemy : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UDecalComponent* DecaleComponent;
	AEnemy();
	
};
