#pragma once

#define OCCUPIED -1
#define FREE 0

#define MAX_OBJECTS_IN_WORLD 64

#include "GameJamGame/Gameplay/World/WorldObject.h"

#include <vector>
#include <type_traits>

namespace tree
{
	class CWorldProbe;

	using FWorldObjectID = int;

	class CWorldState
	{
	public:
		CWorldState();

		template<typename ObjectType>
		FWorldObjectID AddToWorld(const sf::Vector2f& InAtPostion, const sf::Vector2f& InBounds);
		
		void RemoveFromWorld(FWorldObjectID InIDToRemove);

		void SetWorldObjectPosition(FWorldObjectID InID, const sf::Vector2f& InPosition);

		void ProbeForData(CWorldProbe& InProbe) const;
	private:
		SWorldObject WorldObjects[MAX_OBJECTS_IN_WORLD];
		int FreeIndexes[MAX_OBJECTS_IN_WORLD];
		int LastPotentiallyValidIndex;
	};
}

template<typename ObjectType>
tree::FWorldObjectID tree::CWorldState::AddToWorld(const sf::Vector2f& InAtPostion, const sf::Vector2f& InBounds)
{
	int Index = -1;

	for (int i = 0; i < MAX_OBJECTS_IN_WORLD; ++i)
	{
		if (FreeIndexes[i] == FREE)
		{
			Index = i;
			break;
		}
	}

	WorldObjects[Index].Position = InAtPostion;
	WorldObjects[Index].Bounds = InBounds;
	WorldObjects[Index].ObjectType = typeid(ObjectType).hash_code();
	FreeIndexes[Index] = OCCUPIED;
	if (Index > LastPotentiallyValidIndex)
	{
		LastPotentiallyValidIndex = Index;
	}

	return FWorldObjectID(Index);
}