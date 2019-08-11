#pragma once
#include <SFML/System/Vector2.hpp>

namespace tree
{
	class IController;

	class CPawn
	{
	public:
		CPawn();

		void SetSpeed(float InSpeed);
		float GetSpeed() const;

		void AttachController(const IController& InController);
		const IController& GetController() const;
		virtual void Tick(float InDT);

		void SetPositon(const sf::Vector2f& InPosition);
		const sf::Vector2f& GetPosition() const;
	protected:
		sf::Vector2f Position;
		float Speed;
		const IController* Controller;
	};
}