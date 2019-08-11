#include "GameJamGame/Gameplay/World/ProbeConstructor.h"

#include <SpelSylt/Debugging/Logging/DebugLogger.h>
#include <cassert>

//------------------------------------------------------------------

#define MAX_PROBES 64

//------------------------------------------------------------------

using namespace tree;

const CWorldState* CProbeConstructor::WorldState;
std::vector<CWorldProbe> CProbeConstructor::Probes;

//------------------------------------------------------------------

CProbeConstructor::CProbeConstructor(const CWorldState& InWorldState)
{
	WorldState = &InWorldState;
	Probes.reserve(MAX_PROBES);
}

//------------------------------------------------------------------

CWorldProbe& CProbeConstructor::ConstructProbe()
{
#ifdef _DEBUG
	if (Probes.max_size() == MAX_PROBES)
	{
		LOG_ERROR(CProbeConstructor, "Probes vector will resize (and break refs to constructed probes) consider increasing the MAX_PROBES macro");
		assert(false);
	}
#endif

	Probes.emplace_back(*WorldState);
	return Probes.back();
}

//------------------------------------------------------------------
