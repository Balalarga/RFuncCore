// Fill out your copyright notice in the Description page of Project Settings.
#include "MImageSpawner.h"

#include "Executor/OpenclExecutor.h"
#include "Language/Parser.h"
#include "Language/Generators/OpenclGenerator.h"
#include "Utils/FlatArray.h"
#include "VoxelCore/Space/MultiDimSpace.h"


bool GetMImageFromCode(const std::string_view& code, const Ranok::MultiDimSpace& space, FlatArray<Ranok::MImage3D>& outArray);


void UMImageSpawner::Spawn(TSubclassOf<AActor> SpawnType)
{
	FlatArray<Ranok::MImage3D> MImage;
	auto Code = R"(
def main(space[3])
{
	r = 100;
	// Sphere
	return r^2 - space[0]^2 - space[1]^2 - space[2]^2;
})";

	const std::vector<double> SpaceCenter{0, 0, 0};
	const std::vector<double> SpaceSize{200, 200, 200};
	const int SpaceDivideRecursion = 5; // 2^4 voxels per dimension
	const Ranok::MultiDimSpace Space(SpaceCenter, SpaceSize, SpaceDivideRecursion);
	const bool bOk = GetMImageFromCode(Code, Space, MImage);
	if (!bOk)
	{
		return;
	}
	
	UWorld* World = GetWorld();
	for (size_t Index = 0; Index != MImage.Size(); ++Index)
	{
		if (MImage[Index].zone != 0)
			continue;

		std::vector<float> LocationArr = Space.GetPoint(Index);
		FVector Location{LocationArr[0], LocationArr[1], LocationArr[2]};
		AActor* Actor = World->SpawnActor<AActor>(SpawnType, Location, FRotator::ZeroRotator);
	}
}


bool GetMImageFromCode(const std::string_view& code, const Ranok::MultiDimSpace& space, FlatArray<Ranok::MImage3D>& outArray)
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
	cl_uint3 clSpaceSize{{static_cast<unsigned>(space.GetPartition()[0]),
	                      static_cast<unsigned>(space.GetPartition()[1]),
	                      static_cast<unsigned>(space.GetPartition()[2])}};

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
	const int executeStatus = exec.ExecuteCurrentKernel(Ranok::OpenclGenerator::sKernelProgram,
	                                                    Ranok::Opencl::KernelArguments(clResult, kernelOptions));
	if (executeStatus != CL_SUCCESS)
	{
		// std::cerr << std::format("OpenCL execution error: '{}'\n", executeStatus);
		return false;
	}
	
	return true;
}