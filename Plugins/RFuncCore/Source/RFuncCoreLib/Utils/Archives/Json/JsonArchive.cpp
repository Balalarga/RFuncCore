#include "JsonArchive.h"
#include "Log/Logger.h"
#include "format"

namespace Ranok
{
JsonArchive::JsonArchive(std::string filepath, ArchiveMode mode):
	FileArchive(filepath, mode)
{
	
}
JsonArchive::~JsonArchive()
{
	JsonArchive::Close();
}

bool JsonArchive::Open()
{
	if (!FileArchive::Open())
		return false;

	if (GetMode() == ArchiveMode::Read)
	{
		try
		{
			_json = nlohmann::json::parse(GetStdStream());
		}
		catch (...)
		{
			return false;
		}
		return !_json.is_null();
	}
	
	if (GetMode() == ArchiveMode::Write)
	{
		_json = nlohmann::json::object();
	}

	return true;
}

void JsonArchive::Close()
{
	if (GetMode() == ArchiveMode::Write)
	{
		if (!_json.is_null())
			Write(_json.dump(4));
		else
			Logger::Warning(std::format("Couldn't write to {}", GetFilepath()));
	}
	
	FileArchive::Close();
}
}
