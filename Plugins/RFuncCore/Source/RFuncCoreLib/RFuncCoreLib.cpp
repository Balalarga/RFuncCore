#include "RFuncCoreLib.h"

#include "Executor/OpenclExecutor.h"


#define LOCTEXT_NAMESPACE "FRFuncCoreLibModule"

DEFINE_LOG_CATEGORY_STATIC(LogRFuncCoreLib, Log, All);


void FRFuncCoreLibModule::StartupModule()
{
	if (!ensure(Ranok::Opencl::Executor::Init() == CL_SUCCESS))
	{
		UE_LOG(LogRFuncCoreLib, Error, TEXT("Cannot init OpenCL!!"));
	}
	UE_LOG(LogRFuncCoreLib, Log, TEXT("OpenCL is loaded"));
}

void FRFuncCoreLibModule::ShutdownModule()
{
	Ranok::Opencl::Executor::Destroy();
}


#undef LOCTEXT_NAMESPACE


IMPLEMENT_MODULE(FRFuncCoreLibModule, RFuncCoreLib)
