#include "ZombieSmoke.h"

#include <SpelSylt/Contexts/GameContext.h>
#include <SpelSylt/FileHandling/Asset/AssetManager.h>
#include <SpelSylt/Rendering/RenderQueue.h>
#include <SpelSylt/Math/CommonMath.h>

tree::ZombieSmoke::ZombieSmoke(SpelSylt::SGameContext & aContext)
{
	for (int i = 0; i < 62; ++i)
	{
		mySmokes.push_back(SS::CSprite());
		mySmokes.back().SetTextureAsset(aContext.AssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/smoke.png"));
		mySmokes.back().setPosition(-(960.f+64.f), -(540+64.f));
		mySmokes.back().setOrigin(64, 64);
		if (i < 12)
		{
			float offset = i;
			mySmokes.back().move(0, offset * 112.f);
		}
		else if (i < 24)
		{
			float offset = i - 12;
			mySmokes.back().move(1920.f+128.f, offset * 112.f);
		}
		else if (i < 24 + 19)
		{
			float offset = i - 24;
			mySmokes.back().move(offset * 112.f, 0);
		}
		else if (i < 24 + 19 + 19)
		{
			float offset = i - (24 + 19);
			mySmokes.back().move(offset * 112.f, 1080.f+128.f);
		}
		mySmokes.back().setColor(sf::Color(0, 255, 0, 50 + 150.f * (float)rand() / RAND_MAX));
		mySmokes.back().setRotation(90.f * rand());
	}
}

void tree::ZombieSmoke::Update(float aDT)
{
	for (auto& sprite : mySmokes)
	{
		auto c = sprite.getColor();
		float a = c.a;
		a = Math::Clamp(a + 100.f * aDT, 50.f, 200.f);
	}
}

void tree::ZombieSmoke::Render(SpelSylt::CRenderQueue & aRenderQueue)
{
	for (auto& sprite : mySmokes)
	{
		aRenderQueue.Enqueue(ERenderLayer::UI, SS::SSpriteRenderCommand(sprite));
	}
}
