// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "FuncModelAsyncCalculator.h"
#include "MImageAsyncCalculator.h"
#include "UObject/Object.h"
#include "MImageSpawner.generated.h"


/**
 *
 */
UCLASS()
class RFUNCCORE_API UMImageSpawner: public UTickableWorldSubsystem
{
	GENERATED_BODY()


public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	UFUNCTION(BlueprintCallable)
	void MakeMImage(TSubclassOf<AActor> InSpawnType, const FVector& WorldPosition, uint8 Recursions);

	UFUNCTION(BlueprintCallable)
	void MakeVoxelModel(TSubclassOf<AActor> InSpawnType, const FVector& WorldPosition, uint8 Recursions);


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> SpawnType;


protected:
	void CalculateMImage(const FRnkSpace& Space, TFunction<float(const FVector&)>&& Func, uint8 Recursions);
	void Calculate(const FRnkSpace& Space, TFunction<float(const FVector&)>&& Func, uint8 Recursions);


private:
	TUniquePtr<FMimageAsyncCalculator> MImageCalculator;
	TUniquePtr<FFuncModelAsyncCalculator> ModelAsyncCalculator;
	TMpscQueue<FMimage3D> MImageElements;
	TMpscQueue<FPointZone> ModelVoxels;
	FVector ModelLocation = FVector::ZeroVector;
};
