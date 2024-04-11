#include "RFuncCoreLib.h"

#include "Executor/OpenclExecutor.h"


#define LOCTEXT_NAMESPACE "FRFuncCoreLibModule"


void FRFuncCoreLibModule::StartupModule()
{
	ensure(Ranok::Opencl::Executor::Init() != CL_SUCCESS);
}

void FRFuncCoreLibModule::ShutdownModule()
{
	Ranok::Opencl::Executor::Destroy();
}


#undef LOCTEXT_NAMESPACE


IMPLEMENT_MODULE(FRFuncCoreLibModule, RFuncCoreLib)