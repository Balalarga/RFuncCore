#pragma once
#include "MImage.h"


class FMimageAsyncCalculator: public FRunnable
{
public:
	FMimageAsyncCalculator(const FRnkSpace& InSpace,
		TFunction<float(const FVector&)>&& InFunc,
		uint8 InRecursions,
		TMpscQueue<FMimage3D>& MImageElements);
	virtual ~FMimageAsyncCalculator() override;
	virtual uint32 Run() override;


private:
	TUniquePtr<FRunnableThread> Thread;
	TMpscQueue<FMimage3D>& Queue;
	FRnkSpace Space;
	TFunction<float(const FVector&)> Func;
	uint8 Recursions;
};
