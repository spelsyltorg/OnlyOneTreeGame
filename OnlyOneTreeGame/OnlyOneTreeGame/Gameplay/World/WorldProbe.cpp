#include "GameJamGame/Gameplay/World/WorldProbe.h"

#include "GameJamGame/Gameplay/World/WorldState.h"

//------------------------------------------------------------------

using namespace tree;

//------------------------------------------------------------------

CWorldProbe::CWorldProbe(const CWorldState& InProbeTarget)
	: ProbeTarget(&InProbeTarget)
	, ProbedObjects()
{
}

//------------------------------------------------------------------

void CWorldProbe::ProbeForData()
{
	ProbeTarget->ProbeForData(*this);
}

//------------------------------------------------------------------

const std::set<size_t>& CWorldProbe::GetFilters() const
{
	return Filters;
}

//------------------------------------------------------------------

void tree::CWorldProbe::ClearObjects()
{
	ProbedObjects.clear();
}

//------------------------------------------------------------------

void CWorldProbe::AddObject(const SWorldObject& InObject)
{
	ProbedObjects.emplace_back(InObject);
}

//------------------------------------------------------------------

bool CWorldProbe::IsWithinProbeArea(const SWorldObject& InObject) const
{
	return true;
}

//------------------------------------------------------------------

void CWorldProbe::ForEachProbed(const std::function<void(const SWorldObject&)>& InPred)
{
	for (const SWorldObject& Object : ProbedObjects)
	{
		InPred(Object);
	}
}

//------------------------------------------------------------------