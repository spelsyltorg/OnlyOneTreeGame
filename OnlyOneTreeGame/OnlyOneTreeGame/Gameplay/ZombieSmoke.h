#pragma once
#include "GameJamGame/Core/System.h"
#include <SpelSylt/Rendering/Sprite/Sprite.h>

#include <vector>

namespace SpelSylt
{
	struct SGameContext;
}

namespace tree
{
	class ZombieSmoke : public ISystem
	{
	public:
		ZombieSmoke(SpelSylt::SGameContext& aContext);

		virtual void Update(float aDT) override;
		virtual void Render(SpelSylt::CRenderQueue& aRenderQueue) override;

	private:
		std::vector<SS::CSprite> mySmokes;
	};
}