// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MImageSpawner.generated.h"

/**
 * 
 */
UCLASS()
class RFUNCCORE_API UMImageSpawner: public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Spawn(TSubclassOf<AActor> SpawnType);
};
