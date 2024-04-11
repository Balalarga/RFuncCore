#pragma once

namespace Ranok
{
class IArchive;

class RFUNCCORELIB_API ISerializable
{
public:
	virtual ~ISerializable() = default;

	virtual void Serialize(IArchive& archive) = 0;
};
}
