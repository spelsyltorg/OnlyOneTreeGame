#pragma once

#include "GameJamGame/Gameplay/Weapon/Weapon.h"
#include <SFML/System/Vector2.hpp>

#include <SpelSylt/Rendering/Sprite/Texture.h>

namespace SpelSylt
{
	class CRenderQueue;
	class CAssetManager;
	struct SGameContext;
}

namespace tree
{
	class CWeaponSystem;

	class GrenadeLauncher : public IWeapon
	{
	public:
		GrenadeLauncher(SpelSylt::SGameContext& aGameContext);

		virtual void Update(float aDt) override;
		virtual void Render(SpelSylt::CRenderQueue& aRenderQueue) override;
		virtual void Shoot() override;

	private:
		struct SGrenade
		{
			sf::Vector2f start;
			sf::Vector2f end;
			float transition;
			float timeScale;
		};

		std::vector<SGrenade> myActiveGrenades;
		SS::CTexture myGrenadeTexture;
	};
}