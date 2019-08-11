#include "GameJamGame/Gameplay/World/WorldState.h"

#include "GameJamGame/Gameplay/World/WorldProbe.h"

//------------------------------------------------------------------

using namespace tree;

//------------------------------------------------------------------

CWorldState::CWorldState()
	: WorldObjects()
	, FreeIndexes()
	, LastPotentiallyValidIndex(0)
{
	for (int i = 0; i < MAX_OBJECTS_IN_WORLD; ++i)
	{
		FreeIndexes[i] = FREE;
	}
}

//------------------------------------------------------------------

void CWorldState::RemoveFromWorld(FWorldObjectID InIDToRemove)
{
	FreeIndexes[InIDToRemove] = FREE;
}

//------------------------------------------------------------------

void CWorldState::SetWorldObjectPosition(FWorldObjectID InID, const sf::Vector2f& InPosition)
{
	WorldObjects[InID].Position = InPosition;
}

//------------------------------------------------------------------

void CWorldState::ProbeForData(CWorldProbe& InProbe) const
{
	InProbe.ClearObjects();
	const auto& Filters = InProbe.GetFilters();

	for (int i = 0; i < LastPotentiallyValidIndex + 1; ++i)
	{
		if (FreeIndexes[i] == OCCUPIED																//This index contains valid data
			&& InProbe.IsWithinProbeArea(WorldObjects[i])											//This object is withing the probe's range
			&& (Filters.size() == 0 || Filters.find(WorldObjects[i].ObjectType) != Filters.end()))	//The probe is filtering this type in or have no set filters
		{
				InProbe.AddObject(WorldObjects[i]);
		}
	}
}

//------------------------------------------------------------------
