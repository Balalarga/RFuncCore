#pragma once
#include <optional>
#include <map>

#include "Utils/FileUtils.h"
#include "ActionNode.h"
#include "Parser.h"

namespace Ranok
{
std::optional<ActionNodeFactory> RFUNCCORELIB_API LoadLibrary(const std::string& path);
std::map<std::string, ActionNodeFactory> RFUNCCORELIB_API LoadDefaultLibraries();
}
