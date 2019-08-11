#include "GrenadeLauncher.h"
#include <SpelSylt/Contexts/GameContext.h>
#include <SpelSylt/Messaging/MessageQueue.h>
#include <SpelSylt/Messaging/Messages/AudioMessages.h>
#include <SpelSylt/Utility/Input/InputEventGetter.h>
#include <SpelSylt/FileHandling/Asset/AssetManager.h>
#include <SpelSylt/Rendering/Sprite/Sprite.h>
#include <SpelSylt/Math/CommonMath.h>
#include <SpelSylt/Rendering/RenderQueue.h>

#include "GameJamGame/Core/GameMessages.h"
#include "GameJamGame/Gameplay/Weapon/WeaponSystem.h"
#include "GameJamGame/Core/AnimationSequencer.h"

#include "GameJamGame/Core/WindowDefines.h"

tree::GrenadeLauncher::GrenadeLauncher(SpelSylt::SGameContext & aGameContext) : 
	IWeapon(aGameContext)
{
	myGrenadeTexture = aGameContext.AssetManager.GetAsset<STextureAsset>("Graphics/Sprites/grenade.png");

	myAmmo = 0;
	myTimer.SetDuration(1.5f);
	myContext.MessageQueue.DispatchEvent<GrenadeAmmoMsg>(myAmmo);
}

void tree::GrenadeLauncher::Update(float aDt)
{
	IWeapon::Update(aDt);

	for (int i = 0; i < myActiveGrenades.size(); ++i)
	{
		auto& grenade = myActiveGrenades[i];

		grenade.transition = Math::Min(1.f, grenade.transition + aDt / grenade.timeScale);

		if (grenade.transition == 1.f)
		{
			myContext.MessageQueue.DispatchEvent<HitscanExplosionMsg>(grenade.end, 200.f);
			myContext.MessageQueue.DispatchEvent<SSoundMessage>("explosion");
			CAnimationSequencer::PlayAnimationAtPosition("Graphics/Animations/explosion.anmbndl", grenade.end);
			grenade = myActiveGrenades.back();
			myActiveGrenades.pop_back();
		}
	}
}

void tree::GrenadeLauncher::Render(SpelSylt::CRenderQueue & aRenderQueue)
{
	SS::CSprite sprite;
	sprite.SetTextureAsset(myGrenadeTexture.Get());
	sprite.setOrigin(32, 32);

	for (auto& grenade : myActiveGrenades)
	{
		sprite.setRotation(360.f * grenade.transition);
		sprite.setPosition(Math::Lerp(grenade.start, grenade.end, grenade.transition));
		float amp = Math::Length(grenade.end - grenade.start) / 500.f;
		float scale =  0.75f + amp * sinf(3.14f*grenade.transition);
		sprite.setScale(scale, scale);

		aRenderQueue.Enqueue(ERenderLayer::Foreground, SSpriteRenderCommand(sprite));
	}
}

void tree::GrenadeLauncher::Shoot()
{
	if (myAmmo == 0)
		return;

	sf::Vector2f from = WeaponSystem->GetPlayerPos();
	sf::Vector2f to = WeaponSystem->GetAimPos();

	SGrenade grenade;
	grenade.start = from;
	grenade.end = to;
	grenade.timeScale = Math::Length(to - from) / 750.f; // Speed
	grenade.transition = 0.f;

	myActiveGrenades.emplace_back(grenade);

	myTimer.Restart();
	myCanFire = false;

	if (myAmmo > 0)
	{
		--myAmmo;
	}

	myContext.MessageQueue.DispatchEvent<GrenadeAmmoMsg>(myAmmo);
	myContext.MessageQueue.DispatchEvent<SSoundMessage>("grenade_launcher");
}
