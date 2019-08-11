#pragma once

#include "GameJamGame/Gameplay/Weapon/HitScanWeapon.h"
#include <SFML/Audio/Sound.hpp>

namespace tree
{
	class MiniGun : public IHitScanWeapon
	{
	public:
		MiniGun(SpelSylt::SGameContext& aGameContext);

		virtual void Update(float aDT) override;
		virtual void PrepareForShoot() override;

		virtual void OnActivated() override;

	private:
		float myCooldown;
		sf::Sound mySound;
	};
}