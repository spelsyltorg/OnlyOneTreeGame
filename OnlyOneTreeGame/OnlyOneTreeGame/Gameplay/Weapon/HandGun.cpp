#include "HandGun.h"
#include "GameJamGame/Gameplay/Weapon/WeaponSystem.h"

#include <SpelSylt/Math/CommonMath.h>
#include <SpelSylt/Contexts/GameContext.h>
#include <SpelSylt/Messaging/MessageQueue.h>
#include <SpelSylt/Messaging/Messages/AudioMessages.h>
#include "GameJamGame/Core/GameMessages.h"

tree::HandGun::HandGun(SpelSylt::SGameContext & aGameContext) :
	IHitScanWeapon(aGameContext)
{
	myRange = 700.f;
	myTimer.SetDuration(0.5f);
	myAmmo = -1;
}

void tree::HandGun::PrepareForShoot()
{
	sf::Vector2f p = WeaponSystem->GetPlayerPos();
	sf::Vector2f a = WeaponSystem->GetAimPos();

	auto dir = a - p;
	float l = Math::Length(dir);
	sf::Vector2f spread(dir.y, -dir.x);
	Math::Normalize(spread);
	float s = 50.f * (l / myRange);

	int shots = Math::Min(myAmmo, 1);
	if (myAmmo < 0)
		shots = 1;

	float amp = s * (-1.f + 2.f * ((rand() % 100) / 100.f));

	AddShot(p, a + amp * spread);
	if (myAmmo > 0)
		--myAmmo;

	std::string alias = "handgun";

	myContext.MessageQueue.DispatchEvent<SSoundMessage>(alias);
}
