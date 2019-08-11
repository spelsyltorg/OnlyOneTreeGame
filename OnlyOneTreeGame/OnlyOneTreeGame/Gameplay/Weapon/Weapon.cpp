#include "Weapon.h"
#include <SpelSylt/Math/CommonMath.h>

#include <SpelSylt/Contexts/GameContext.h>
#include <SpelSylt/Messaging/MessageQueue.h>
#include "GameJamGame/Core/GameMessages.h"

tree::IWeapon::IWeapon(SpelSylt::SGameContext& aGameContext) :
	myContext(aGameContext),
	myCanFire(true),
	myIsLocked(false),
	myAmmo(-1)
{
	myTimer.Init(CTimedEvent::EType::Repeat, 0.f, [this]() {
		myCanFire = true;
	});
	myTimer.Start();

	aGameContext.MessageQueue.Subscribe<LockWeaponsMsg>([this](auto msg) { myIsLocked = msg.Param; }, mySubscriptions);
}

void tree::IWeapon::Update(float aDt)
{
	myTimer.Update(aDt);
}

void tree::IWeapon::SetWeaponSystem(CWeaponSystem * aWeaponSystem)
{
	WeaponSystem = aWeaponSystem;
}

int tree::IWeapon::AddAmmo(int aAmmo)
{
	myAmmo = Math::Min(999, myAmmo + aAmmo);
	return myAmmo;
}
