#pragma once
#include "GameJamGame/Gameplay/Weapon/Weapon.h"
#include <SpelSylt/Rendering/Sprite/Texture.h>

#include <SFML/System/Vector2.hpp>

#include <vector>

namespace SpelSylt
{ 
	struct SGameContext;
}

namespace tree
{
	typedef std::pair<sf::Vector2f, sf::Vector2f> HitScanShot;

	class IHitScanWeapon : public IWeapon
	{
	public:
		IHitScanWeapon(SpelSylt::SGameContext& aGameContext);

		virtual void Shoot() override final;
		virtual void Update(float aDT) override;
		virtual void Render(SpelSylt::CRenderQueue& aRenderQueue) override;

		virtual void PrepareForShoot() {}

		void AddShot(const sf::Vector2f& aFrom, const sf::Vector2f& aTo);

	protected:
		float myRange;

	private:
		std::vector<HitScanShot> myShotsThisFrame;

		struct SFadingShot
		{
			HitScanShot line;
			float fade;
		};
		std::vector<SFadingShot> myFadingShots;

		SpelSylt::CTexture myTexture;
	};
}