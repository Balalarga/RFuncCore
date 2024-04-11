// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RnkController.generated.h"


UCLASS()
class RANOKVISUALIZER_API ARnkController: public APlayerController
{
	GENERATED_BODY()


public:


protected:
	virtual void SetupInputComponent() override;
};
