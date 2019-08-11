#pragma once
#include <vector>

#include "GameJamGame/Gameplay/World/WorldProbe.h"

namespace tree
{
	class CWorldState;

	class CProbeConstructor
	{
	public:
		CProbeConstructor(const CWorldState& InWorldState);
		static CWorldProbe& ConstructProbe();
	private:
		static std::vector<CWorldProbe> Probes;
		static const CWorldState* WorldState;
	};
}