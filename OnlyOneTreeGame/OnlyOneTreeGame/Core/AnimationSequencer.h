#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include <SFML/System/Vector2.hpp>

#include "GameJamGame/Core/System.h"

#include <SpelSylt/Rendering/Animation/SpriteAnimation.h>

namespace SpelSylt
{
	class CAssetManager;
}

namespace tree
{
	class CAnimationSequencer
		: public ISystem
	{
	public:
		CAnimationSequencer(SpelSylt::CAssetManager& InAssetManager);

		static SS::CSpriteAnimation& PlayAnimationAtPosition(const char* InAnimation, const sf::Vector2f& InPosition);

		void Update(float aDT) override;
		void Render(SpelSylt::CRenderQueue& aRenderQueue) override;

	private:
		static SpelSylt::CAssetManager* AssetManager;
		static std::vector<SS::CSpriteAnimation> Animations;

	};
}