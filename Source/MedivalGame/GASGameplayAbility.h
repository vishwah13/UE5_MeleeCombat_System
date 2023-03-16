// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MedivalGame.h"
#include "GASGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class MEDIVALGAME_API UGASGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	UGASGameplayAbility();
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "Ability")
	EGASAbilityInputID AbilityInputID = EGASAbilityInputID::None;
};
