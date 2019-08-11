#include "MiniGun.h"
#include "GameJamGame/Gameplay/Weapon/WeaponSystem.h"

#include <SpelSylt/Math/CommonMath.h>
#include <SpelSylt/Contexts/GameContext.h>
#include <SpelSylt/Messaging/MessageQueue.h>
#include <SpelSylt/Messaging/Messages/AudioMessages.h>
#include "GameJamGame/Core/GameMessages.h"
#include <SpelSylt/Utility/Input/InputEventGetter.h>
#include <SpelSylt/Audio/AudioManager.h>

tree::MiniGun::MiniGun(SpelSylt::SGameContext & aGameContext) :
	IHitScanWeapon(aGameContext)
{
	myRange = 900.f;
	myTimer.SetDuration(.15f);
	myAmmo = 0;

	myCooldown = 2.f;
	myContext.MessageQueue.DispatchEvent<MinigunAmmoMsg>(myAmmo);
}

void tree::MiniGun::Update(float aDT)
{
	if (myContext.Input.IsKeyDown(EKeyCode::MouseLeft))
	{
		myCooldown -= aDT;
	}
	else
	{
		myCooldown += aDT;
	}

	myCooldown = Math::Clamp(myCooldown, 0.f, 2.f);

	IHitScanWeapon::Update(aDT);
}

void tree::MiniGun::PrepareForShoot()
{
	if (mySound.getStatus() != SoundSource::Playing)
	{
		myContext.AudioManager.PlaySound("minigun_load", &mySound);
	}

	if (myCooldown > 1.f)
		return;

	mySound.stop();

	sf::Vector2f p = WeaponSystem->GetPlayerPos();
	sf::Vector2f a = WeaponSystem->GetAimPos();

	auto dir = a - p;
	float l = Math::Length(dir);
	sf::Vector2f spread(dir.y, -dir.x);
	Math::Normalize(spread);
	float s = 75 * (l / myRange);

	int shots = Math::Min(myAmmo, 3);
	if (myAmmo < 0)
		shots = 3;
	for (int i = 0; i < shots; ++i)
	{
		float amp = s * (-1.f + 2.f * ((rand() % 100) / 100.f));

		AddShot(p, a + amp * spread);
		if (myAmmo > 0)
			--myAmmo;
	}

	std::string alias = "shotgun";
	if (shots <= 0)
		alias.append("_empty");

	if (shots > 0)
		myContext.MessageQueue.DispatchEvent<SSoundMessage>("minigun");

	myContext.MessageQueue.DispatchEvent<MinigunAmmoMsg>(myAmmo);
}

void tree::MiniGun::OnActivated()
{
	myCooldown = 2.f;
}
