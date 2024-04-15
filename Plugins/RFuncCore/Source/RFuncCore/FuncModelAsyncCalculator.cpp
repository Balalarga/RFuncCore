#include "FuncModelAsyncCalculator.h"

FFuncModelAsyncCalculator::FFuncModelAsyncCalculator(const FRnkSpace& InSpace,
	TFunction<float(const FVector&)>&& InFunc,
	uint8 InRecursions,
	TMpscQueue<FPointZone>& MImageElements): Queue(MImageElements)
	                                       , Space(InSpace)
	                                       , Func(MoveTemp(InFunc))
	                                       , Recursions(InRecursions)
{
	Thread.Reset(FRunnableThread::Create(this, TEXT("RFuncThread")));
}

inline FMatrix CreateMatrix(const FVector4& X, const FVector4& Y, const FVector4& Z, const FVector4& W)
{
	FMatrix Matrix;
	Matrix.M[0][0] = X.X;
	Matrix.M[1][0] = X.Y;
	Matrix.M[2][0] = X.Z;
	Matrix.M[3][0] = X.W;

	Matrix.M[0][1] = Y.X;
	Matrix.M[1][1] = Y.Y;
	Matrix.M[2][1] = Y.Z;
	Matrix.M[3][1] = Y.W;

	Matrix.M[0][2] = Z.X;
	Matrix.M[1][2] = Z.Y;
	Matrix.M[2][2] = Z.Z;
	Matrix.M[3][2] = Z.W;

	Matrix.M[0][3] = W.X;
	Matrix.M[1][3] = W.Y;
	Matrix.M[2][3] = W.Z;
	Matrix.M[3][3] = W.W;
	return Matrix;
}

FFuncModelAsyncCalculator::~FFuncModelAsyncCalculator()
{
	if (Thread)
	{
		Thread->Kill(true);
	}
}

uint32 FFuncModelAsyncCalculator::Run()
{
	const int32 AxisSplit = 1 << (Recursions - 1);
	const int32 TotalSplits = AxisSplit * AxisSplit * AxisSplit;
	const FVector SpaceStep = Space.HalfSize * 2 / AxisSplit;
	const FVector SpaceBegin = Space.Center - Space.HalfSize + SpaceStep / 2;
	const FVector4 SpaceEnd = Space.Center + Space.HalfSize;

	SpentTime = FApp::GetCurrentTime();
	// W component used separately, be careful with vector operations
	int32 Counter = 0;
	auto Compute = [this, &Counter, &SpaceStep](FVector Loc)
	{
		FVector4 Neighbours[]
		{
			Loc + FVector{SpaceStep.X, 0, 0},
			Loc + FVector{0, SpaceStep.Y, 0},
			Loc + FVector{0, 0, SpaceStep.Z},
			Loc - FVector{SpaceStep.X, 0, 0},
			Loc - FVector{0, SpaceStep.Y, 0},
			Loc - FVector{0, 0, SpaceStep.Z},

			Loc + FVector{SpaceStep.X, SpaceStep.Y, 0},
			Loc + FVector{0, SpaceStep.Y, SpaceStep.Z},
			Loc + FVector{SpaceStep.X, 0, SpaceStep.Z},
			Loc + FVector{SpaceStep.X, SpaceStep.Y, SpaceStep.Z},

			Loc - FVector{SpaceStep.X, SpaceStep.Y, 0},
			Loc - FVector{0, SpaceStep.Y, SpaceStep.Z},
			Loc - FVector{SpaceStep.X, 0, SpaceStep.Z},
			Loc - FVector{SpaceStep.X, SpaceStep.Y, SpaceStep.Z},
		};
		// Simple version with direct aligned neighbours
		// FVector4 Neighbours[]
		// {
		// 	Loc + FVector{SpaceStep.X, 0, 0},
		// 	Loc + FVector{0, SpaceStep.Y, 0},
		// 	Loc + FVector{0, 0, SpaceStep.Z},
		// 	Loc - FVector{SpaceStep.X, 0, 0},
		// 	Loc - FVector{0, SpaceStep.Y, 0},
		// 	Loc - FVector{0, 0, SpaceStep.Z},
		// };

		ERFuncZone Zone = ERFuncZone::Zero;
		for (auto& Neighbour: Neighbours)
		{
			Neighbour.W = Func(Neighbour);
			if (Neighbour.W > UE_KINDA_SMALL_NUMBER)
			{
				if (Zone == ERFuncZone::Zero)
				{
					Zone = ERFuncZone::Positive;
				}
				else if (Zone == ERFuncZone::Negative)
				{
					Queue.Enqueue(Loc, ERFuncZone::Zero);
					break;
				}
			}
			if (Neighbour.W < -UE_KINDA_SMALL_NUMBER)
			{
				if (Zone == ERFuncZone::Zero)
				{
					Zone = ERFuncZone::Negative;
				}
				else if (Zone == ERFuncZone::Positive)
				{
					Queue.Enqueue(Loc, ERFuncZone::Zero);
					break;
				}
			}
		}

		++Counter;
	};

	FVector Loc0;
	Loc0.X = SpaceBegin.X;
	while (Loc0.X <= SpaceEnd.X)
	{
		Loc0.Y = SpaceBegin.Y;
		while (Loc0.Y <= SpaceEnd.Y)
		{
			Loc0.Z = SpaceBegin.Z;
			while (Loc0.Z <= SpaceEnd.Z)
			{
				Compute(Loc0);
				Loc0.Z += SpaceStep.Z;
			}
			Loc0.Y += SpaceStep.Y;
		}
		Loc0.X += SpaceStep.X;
	}
	UE_LOG(
		LogTemp,
		Display,
		TEXT("Model computation time: %fsec (%d points)"),
		FApp::GetCurrentTime() - SpentTime,
		TotalSplits);
	return 0;
}
