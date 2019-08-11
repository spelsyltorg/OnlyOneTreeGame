#pragma once
#include <SFML/System/Vector2.hpp>

namespace tree
{
	class CPawn;

	class IController
	{
	public:
		virtual void RegisterPawn(const CPawn& InPawn) const = 0; //Const because stupid me ensured pawns only have const refs to controllers :)
		virtual void ResetTarget() const {};
		virtual void Update() = 0;
		virtual const sf::Vector2f& GetVelocity() const = 0;
	};
}