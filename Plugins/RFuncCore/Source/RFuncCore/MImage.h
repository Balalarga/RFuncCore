#pragma once
#include "MImage.generated.h"


UENUM()
enum class ERFuncZone: uint8
{
	Negative,
	Zero,
	Positive,
};


USTRUCT()
struct FPointZone
{
	GENERATED_BODY()

	FVector Location;
	ERFuncZone Zone;
};


USTRUCT()
struct FMimage3D: public FPointZone
{
	GENERATED_BODY()

	TArray<double, TInlineAllocator<5>> Image;
};


USTRUCT()
struct FRnkSpace
{
	GENERATED_BODY()

	FVector Center;
	FVector HalfSize;
};
