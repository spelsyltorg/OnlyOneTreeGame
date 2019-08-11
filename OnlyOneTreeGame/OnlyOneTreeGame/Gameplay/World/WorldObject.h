#pragma once
#include <SFML/System/Vector2.hpp>

namespace tree
{
	struct SWorldObject
	{
		size_t ObjectType;
		sf::Vector2f Position;
		sf::Vector2f Bounds;
	};
}