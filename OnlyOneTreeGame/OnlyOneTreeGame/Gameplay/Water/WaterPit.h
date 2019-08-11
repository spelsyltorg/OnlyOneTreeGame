#pragma once

#include <SpelSylt/Rendering/Animation/SpriteAnimation.h>
#include <SpelSylt/Rendering/Sprite/Sprite.h>


namespace SpelSylt
{
	class CRenderQueue;
	class CAssetManager;
}

namespace tree
{
	class CWaterPit
	{
	public:
		CWaterPit(SpelSylt::CAssetManager& InAssetManager);

		void SetPosition(const sf::Vector2f& InPosition);
		const sf::Vector2f& GetPosition() const;

		void Update(float InDT);
		void Render(SpelSylt::CRenderQueue& aRenderQueue);
		
		void TakeWater();
		void SetFilling();
		bool IsFull() const;
	private:
		SS::CSpriteAnimation FillingAnimation;
		SS::CSpriteAnimation FullAnimation;
		SS::CSprite PitSprite;

		sf::Vector2f Position;

		bool Filling : 1;
		bool Full : 1;
	};
}