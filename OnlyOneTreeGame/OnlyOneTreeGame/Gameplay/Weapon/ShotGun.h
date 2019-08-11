#pragma once

#include "GameJamGame/Gameplay/Weapon/HitScanWeapon.h"

namespace tree
{
	class ShotGun : public IHitScanWeapon
	{
	public:
		ShotGun(SpelSylt::SGameContext& aGameContext);

		virtual void PrepareForShoot() override;
	};
}