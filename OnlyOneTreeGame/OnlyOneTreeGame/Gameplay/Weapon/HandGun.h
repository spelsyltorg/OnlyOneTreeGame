#pragma once

#include "GameJamGame/Gameplay/Weapon/HitScanWeapon.h"

namespace tree
{
	class HandGun : public IHitScanWeapon
	{
	public:
		HandGun(SpelSylt::SGameContext& aGameContext);

		virtual void PrepareForShoot() override;
	};
}