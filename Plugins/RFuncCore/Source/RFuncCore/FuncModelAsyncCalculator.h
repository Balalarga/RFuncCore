#pragma once
#include "MImage.h"


class FFuncModelAsyncCalculator: public FRunnable
{
public:
	FFuncModelAsyncCalculator(const FRnkSpace& InSpace,
		TFunction<float(const FVector&)>&& InFunc,
		uint8 InRecursions,
		TMpscQueue<FPointZone>& Voxels);
	virtual ~FFuncModelAsyncCalculator() override;
	virtual uint32 Run() override;


private:
	TUniquePtr<FRunnableThread> Thread;
	TMpscQueue<FPointZone>& Queue;
	FRnkSpace Space;
	TFunction<float(const FVector&)> Func;
	uint8 Recursions;
	double SpentTime = 0;
};
