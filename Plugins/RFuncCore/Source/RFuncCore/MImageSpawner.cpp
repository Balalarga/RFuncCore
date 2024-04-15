// Fill out your copyright notice in the Description page of Project Settings.
#include "MImageSpawner.h"

#include "Executor/OpenclExecutor.h"
#include "Language/Parser.h"
#include "Language/Generators/OpenclGenerator.h"
#include "Utils/FlatArray.h"
#include "VoxelCore/Space/MultiDimSpace.h"


bool GetMImageFromCode(const std::string_view& code,
	const Ranok::MultiDimSpace& space,
	FlatArray<Ranok::MImage3D>& outArray);


void UMImageSpawner::MakeMImage(TSubclassOf<AActor> InSpawnType, const FVector& WorldPosition, uint8 Recursions)
{
	ModelLocation = WorldPosition;
	SpawnType = InSpawnType;
	const FRnkSpace Space({0, 0, 0}, {200, 200, 200});
	CalculateMImage(
		Space,
		[](const FVector& Point) -> float
		{
			constexpr float Radius = 200;
			// Sphere
			return FMath::Pow(Radius, 2) -
				FMath::Pow(Point.X, 2) -
				FMath::Pow(Point.Y, 2) -
				FMath::Pow(Point.Z, 2);
		},
		Recursions);
}

void UMImageSpawner::MakeVoxelModel(TSubclassOf<AActor> InSpawnType, const FVector& WorldPosition, uint8 Recursions)
{
	ModelLocation = WorldPosition;
	SpawnType = InSpawnType;
	const FRnkSpace Space({0, 0, 0}, {200, 200, 200});
	Calculate(
		Space,
		[](const FVector& Point) -> float
		{
			constexpr float Radius = 200;
			// Sphere
			return FMath::Pow(Radius, 2) -
				FMath::Pow(Point.X, 2) -
				FMath::Pow(Point.Y, 2) -
				FMath::Pow(Point.Z, 2);
		},
		Recursions);
}


void UMImageSpawner::CalculateMImage(const FRnkSpace& Space,
	TFunction<float(const FVector&)>&& Func,
	uint8 Recursions)
{
	MImageCalculator = MakeUnique<FMimageAsyncCalculator>(Space, MoveTemp(Func), Recursions, MImageElements);
}

void UMImageSpawner::Calculate(const FRnkSpace& Space, TFunction<float(const FVector&)>&& Func, uint8 Recursions)
{
	ModelAsyncCalculator = MakeUnique<FFuncModelAsyncCalculator>(Space, MoveTemp(Func), Recursions, ModelVoxels);
}

void UMImageSpawner::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
}

void UMImageSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	int32 Counter = 50;
	while (TOptional<FMimage3D> MImage = MImageElements.Dequeue())
	{
		if (Counter == 0)
		{
			break;
		}

		if (MImage->Zone == ERFuncZone::Zero)
		{
			--Counter;
			GetWorld()->SpawnActor<AActor>(SpawnType, ModelLocation + MImage->Location, FRotator::ZeroRotator);
		}
	}
	while (auto Voxel = ModelVoxels.Dequeue())
	{
		if (Counter == 0)
		{
			break;
		}
		if (Voxel->Zone == ERFuncZone::Zero)
		{
			--Counter;
			GetWorld()->SpawnActor<AActor>(SpawnType, ModelLocation + Voxel->Location, FRotator::ZeroRotator);
		}
	}
}

TStatId UMImageSpawner::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMImageSpawner, STATGROUP_Tickables);
}


bool GetMImageFromCode(const std::string_view& code,
	const Ranok::MultiDimSpace& space,
	FlatArray<Ranok::MImage3D>& outArray)
{
	Ranok::Parser parser;
	Ranok::ActionTree programAst = parser.Parse(Ranok::Lexer{code});
	if (parser.HasErrors())
	{
		// for (const auto& error: parser.Errors())
		// 	std::cerr << error.text << std::endl;

		return false;
	}

	Ranok::OpenclGenerator generator;
	std::optional<std::string> openclCode = generator.Generate(programAst);
	if (!openclCode)
	{
		// std::cerr << "OpenCL generation errors\n";
		// for (const auto& error: generator.Errors())
		// 	std::cerr << error << "\n";

		return false;
	}

	Ranok::Opencl::Executor exec;
	if (const int compileStatus = exec.Compile(*openclCode); compileStatus != CL_SUCCESS)
	{
		// std::cerr << std::format("OpenCL compilation error: '{}'\n", compileStatus);
		return false;
	}

	cl_int clStartIndex = 0;
	cl_uint3 clSpaceSize{
		{
			static_cast<unsigned>(space.GetPartition()[0]),
			static_cast<unsigned>(space.GetPartition()[1]),
			static_cast<unsigned>(space.GetPartition()[2])
		}
	};

	cl_double3 clStartPoint{{space.GetStartPoint()[0], space.GetStartPoint()[1], space.GetStartPoint()[2]}};
	cl_double3 clPointSize{{space.GetUnitSize()[0], space.GetUnitSize()[1], space.GetUnitSize()[2]}};
	cl_double3 clHalfSize{{clPointSize.x / 2., clPointSize.y / 2., clPointSize.z / 2.}};

	std::vector<Ranok::Opencl::KernelArguments::Argument> kernelOptions
	{
		{&clStartIndex, sizeof(clStartIndex)},
		{&clSpaceSize, sizeof(clSpaceSize)},
		{&clStartPoint, sizeof(clStartPoint)},
		{&clPointSize, sizeof(clPointSize)},
		{&clHalfSize, sizeof(clHalfSize)},
	};

	size_t totalVoxels = space.GetTotalPartition();
	outArray.Resize(totalVoxels);
	Ranok::Opencl::KernelArguments::Argument clResult(&outArray[0], sizeof(outArray[0]), outArray.Size());
	const int executeStatus = exec.ExecuteCurrentKernel(
		Ranok::OpenclGenerator::sKernelProgram,
		Ranok::Opencl::KernelArguments(clResult, kernelOptions));
	if (executeStatus != CL_SUCCESS)
	{
		// std::cerr << std::format("OpenCL execution error: '{}'\n", executeStatus);
		return false;
	}

	return true;
}
