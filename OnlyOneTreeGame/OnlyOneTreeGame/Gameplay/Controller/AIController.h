#pragma once

#include "GameJamGame/Gameplay/Controller/ControllerInterface.h"

namespace tree
{
	class CWorldProbe;

	class CAIController final
		: public IController
	{
	public:
		CAIController();

		virtual void RegisterPawn(const CPawn& InPawn) const override;
		virtual void Update() override;
		virtual const sf::Vector2f& GetVelocity() const;

		const sf::Vector2f& GetTarget() const;
		void ResetTarget() const;

	private:
		sf::Vector2f Seek(const sf::Vector2f& Direction, const sf::Vector2f& CurrentPosition, const sf::Vector2f& TargetPosition, const float InSpeed, const float InRelevance);
		sf::Vector2f Flee(const sf::Vector2f& Direction, const sf::Vector2f& CurrentPosition, const sf::Vector2f& TargetPosition, const float InSpeed, const float InRelevance);
		sf::Vector2f Wander(const sf::Vector2f& Direction, const sf::Vector2f& CurrentPosition, const sf::Vector2f& TargetPosition, const float InSpeed, const float InRelevance);

		CWorldProbe& Probe;
		mutable const CPawn* OwningPawn;
		mutable bool GotTarget;

		float WanderAngle;

		sf::Vector2f Velocity;
		sf::Vector2f Target;
	};
}