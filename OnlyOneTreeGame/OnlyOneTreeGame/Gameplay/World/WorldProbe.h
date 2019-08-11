#pragma once

#include "GameJamGame/Gameplay/World/WorldObject.h"

#include <vector>
#include <set>
#include <type_traits>

#include <functional>

namespace tree
{
	class CWorldState;

	class CWorldProbe
	{
	public:
		CWorldProbe(const CWorldState& InProbeTarget);

		void ProbeForData();

		template <typename FilterType>
		void AddFilter();

		const std::set<size_t>& GetFilters() const;

		void ClearObjects();
		void AddObject(const SWorldObject& InObject);

		bool IsWithinProbeArea(const SWorldObject& InObject) const;

		void ForEachProbed(const std::function<void(const SWorldObject&)>& InPred);
	private:
		const CWorldState* ProbeTarget;
		std::vector<SWorldObject> ProbedObjects;
		std::set<size_t> Filters;
	};
}

template<typename FilterType>
void tree::CWorldProbe::AddFilter()
{
	Filters.insert(typeid(FilterType).hash_code());
}
