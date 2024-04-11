// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <Utils/FlatArray.h>
#include <VoxelCore/Space/MultiDimSpace.h>

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class RFUNCCORE_API FRFuncCoreModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
