#include "HitScanWeapon.h"
#include <SpelSylt/FileHandling/Asset/AssetManager.h>

#include <SpelSylt/Rendering/Sprite/Sprite.h>
#include <SpelSylt/Math/CommonMath.h>
#include <SpelSylt/Rendering/RenderCommand.h>
#include <SpelSylt/Rendering/RenderQueue.h>
#include <SpelSylt/Contexts/GameContext.h>
#include <SpelSylt/Messaging/MessageQueue.h>
#include "GameJamGame/Core/GameMessages.h"

tree::IHitScanWeapon::IHitScanWeapon(SpelSylt::SGameContext& aGameContext) :
	IWeapon(aGameContext),
	myRange(-1.f)
{
	myTexture = aGameContext.AssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/hitscan.png");
	myTimer.SetDuration(0.5f);
}

void tree::IHitScanWeapon::Shoot()
{
	PrepareForShoot();

	for (auto& line : myShotsThisFrame)
	{
		myContext.MessageQueue.DispatchEvent<HitscanShotMsg>(std::make_pair(line.first, line.second));
	}
	myShotsThisFrame.clear();

	myTimer.Restart();
	myCanFire = false;
}

void tree::IHitScanWeapon::Update(float aDT)
{
	IWeapon::Update(aDT);

	for (int i = 0; i < myFadingShots.size(); ++i)
	{
		auto& shot = myFadingShots[i];
		shot.fade -= aDT / 0.2f;
		if (shot.fade < 0.f)
		{
			shot = myFadingShots.back();
			myFadingShots.pop_back();
		}
	}
}

void tree::IHitScanWeapon::Render(SpelSylt::CRenderQueue & aRenderQueue)
{
	for (auto& shot : myFadingShots)
	{
		sf::Vector2f l = shot.line.second - shot.line.first;
		float length = Math::Length(l);

		SS::CSprite s;
		s.SetTextureAsset(myTexture.Get());
		s.setOrigin(2.5f, 64.f);
		s.setScale(0.5f, length / 64.f);
		s.setPosition(shot.line.first);
		s.setRotation(180.f * (atan2f(l.x, -l.y) / 3.1415f));
		s.setColor(sf::Color(255, 255, 255, sf::Uint8(130.f * shot.fade)));

		aRenderQueue.Enqueue(ERenderLayer::Foreground, SS::SSpriteRenderCommand(s));
	}
}

void tree::IHitScanWeapon::AddShot(const sf::Vector2f & aFrom, const sf::Vector2f & aTo)
{
	sf::Vector2f line = aTo - aFrom;
	float range = 99999.f;
	if (myRange > 0)
	{
		range = myRange;
	}
	Math::Normalize(line);
	line = range * line;

	myShotsThisFrame.emplace_back(std::make_pair(aFrom, aFrom + line));
	myFadingShots.emplace_back(SFadingShot{myShotsThisFrame.back(), 1.f });
}
