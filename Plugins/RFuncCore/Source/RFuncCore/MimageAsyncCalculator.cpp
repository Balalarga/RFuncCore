#include "MImageAsyncCalculator.h"


FMimageAsyncCalculator::FMimageAsyncCalculator(const FRnkSpace& InSpace,
	TFunction<float(const FVector&)>&& InFunc,
	uint8 InRecursions,
	TMpscQueue<FMimage3D>& MImageElements): Queue(MImageElements)
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

FMimageAsyncCalculator::~FMimageAsyncCalculator()
{
	if (Thread)
	{
		Thread->Kill(true);
	}
}


uint32 FMimageAsyncCalculator::Run()
{
	const int32 AxisSplit = 1 << (Recursions - 1);
	const int32 TotalSplits = AxisSplit * AxisSplit * AxisSplit;
	const FVector SpaceStep = Space.HalfSize * 2 / AxisSplit;
	const FVector SpaceBegin = Space.Center - Space.HalfSize + SpaceStep / 2;
	const FVector4 SpaceEnd = Space.Center + Space.HalfSize;

	// W component used separately, be careful with vector operations
	int32 Counter = 0;
	auto Compute = [this, &TotalSplits, &Counter, &SpaceStep](FVector4 Loc)
	{
		++Counter;

		Loc.W = Func(Loc);

		FVector4 Loc1 = Loc;
		Loc1.X += SpaceStep.X;
		Loc1.W = Func(Loc1);

		FVector4 Loc2 = Loc;
		Loc2.Y += SpaceStep.Y;
		Loc2.Z += SpaceStep.Z;
		Loc2.W = Func(Loc2);

		FVector4 Loc3 = Loc;
		Loc3.Z += SpaceStep.Z;
		Loc3.W = Func(Loc3);

		const FMatrix MatrixA = CreateMatrix(
			FVector4{Loc.Y, Loc.Z, Loc.W, 1.0},
			FVector4{Loc1.Y, Loc1.Z, Loc1.W, 1.0},
			FVector4{Loc2.Y, Loc2.Z, Loc2.W, 1.0},
			FVector4{Loc3.Y, Loc3.Z, Loc3.W, 1.0}
			);
		const FMatrix MatrixB = CreateMatrix(
			FVector4{Loc.X, Loc.Z, Loc.W, 1.0},
			FVector4{Loc1.X, Loc1.Z, Loc1.W, 1.0},
			FVector4{Loc2.X, Loc2.Z, Loc2.W, 1.0},
			FVector4{Loc3.X, Loc3.Z, Loc3.W, 1.0}
			);
		const FMatrix MatrixC = CreateMatrix(
			FVector4{Loc.X, Loc.Y, Loc.W, 1.0},
			FVector4{Loc1.X, Loc1.Y, Loc1.W, 1.0},
			FVector4{Loc2.X, Loc2.Y, Loc2.W, 1.0},
			FVector4{Loc3.X, Loc3.Y, Loc3.W, 1.0}
			);
		const FMatrix MatrixD = CreateMatrix(
			FVector4{Loc.X, Loc.Y, Loc.Z, 1.0},
			FVector4{Loc1.X, Loc1.Y, Loc1.Z, 1.0},
			FVector4{Loc2.X, Loc2.Y, Loc2.Z, 1.0},
			FVector4{Loc3.X, Loc3.Y, Loc3.Z, 1.0}
			);
		const FMatrix MatrixE = CreateMatrix(
			Loc,
			Loc1,
			Loc2,
			Loc3
			);

		const double DetA = MatrixA.Determinant();
		const double DetB = MatrixB.Determinant();
		const double DetC = MatrixC.Determinant();
		const double DetD = MatrixD.Determinant();
		const double DetE = MatrixE.Determinant();
		const double Avg = FMath::Sqrt(
			FMath::Pow(DetA, 2) +
			FMath::Pow(DetB, 2) +
			FMath::Pow(DetC, 2) +
			FMath::Pow(DetD, 2) +
			FMath::Pow(DetE, 2));
		// if (FMath::Abs(Avg) > UE_KINDA_SMALL_NUMBER)
		{
			FMimage3D Mimage;
			Mimage.Image.AddZeroed(5);
			if (Loc.W > UE_KINDA_SMALL_NUMBER)
			{
				Mimage.Zone = ERFuncZone::Negative;
			}
			else
			{
				Mimage.Zone = ERFuncZone::Zero;
			}
			Mimage.Location = Loc;
			Mimage.Image[0] = DetA / Avg;
			Mimage.Image[1] = DetB / Avg;
			Mimage.Image[2] = DetC / Avg;
			Mimage.Image[3] = DetD / Avg;
			Mimage.Image[4] = DetE / Avg;
			Queue.Enqueue(Mimage);
		}
	};

	FVector4 Loc0;
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
	return 0;
}
